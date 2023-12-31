; THIS IS AN AUTOGENERATED FILE
    ; WARNING: FOR MACOS ONLY
    ; nasm -f macho64 gen_rc4.asm -o gen_rc4.o && ld -o gen_rc4.macho -macosx_version_min 10.7 -e start gen_rc4.o && ./gen_rc4.macho

    BITS 64

    section .text
    global start

    start:
      push   rbp
      mov    rbp, rsp

      ; PRINT OUTPUT
      push   0xa203a
      mov    DWORD [rsp+0x4],0x0
      push   0x76207965
      mov    DWORD [rsp+0x4],0x65756c61
      push   0x7475706e
      mov    DWORD [rsp+0x4],0x6b206120
      push   0x61656c50
      mov    DWORD [rsp+0x4],0x69206573
      mov    rax,0x2000004
      mov    rdi,0x1
      mov    rsi,rsp
      mov    edx,0x1b
      syscall
      pop rax
      pop rax
      pop rax
      pop rax

      ; GET INPUT
      mov rax, 0x2000003
      mov rdi, 0
      lea rsi, [rel key]
      mov edx, 0x10
      syscall

      ; CHECK 4 BYTES OF KEY
      mov     rdx, QWORD [rel key]
      shr     rdx, 32
      cmp     edx, 0x36395477
      jne     fail

      ; GENERATE SBOX
      lea rcx, [rel key]               ; key: rcx
      mov rdx, strict qword 8          ; L: rdx
      lea r8, [rel sbox]               ; sbox: r8
      call rc4init

      lea rcx, [rel encrypted]          ; bufin
      mov rdx, strict qword 48          ; len
      lea r8, [rel encrypted]           ; bufout
      lea r9, [rel sbox]                ; sbox
      call rc4run
      mov eax, 0x2000004 ; write
      mov rdi, 1         ; std out
      lea rsi, [rel encrypted]
      add rsi, 8
      mov edx, 40
      syscall
      mov rax, qword [rel encrypted]
      xor rbx, rbx
      xor rcx, rcx
      xor rdx, rdx
      xor rdi, rdi
      pop rbp
      ret
    ; PRINT FAIL WHALE
    fail:
      mov     eax, 0x2000004 ; write
      mov     rdi, 1         ; std out
      lea     rsi, [rel msg2]
      mov     edx, 230
      syscall
      xor     eax, eax
      mov     rax, 0x2000001 ; exit
      mov     rdi, 0
      syscall

    rc4init:
      push   rbp
      mov    rbp, rsp
      xor    rax, rax
      mov    r11, 0x20
      mov    r9, 0x808080808080808
      mov    r10, 0x706050403020100
    loop1_s:
      cmp    rax,r11
      je     loop1_e
      mov    QWORD [r8+rax*8], r10
      add    r10, r9
      inc    rax
      jmp    loop1_s
    loop1_e:
      xor    rax,rax
      xor    r9,r9
      xor    r10,r10
    loop2_s:
      movzx  r11, BYTE [r8+rax*1]
      add    r9, r11
      movq   xmm0,r11
      movzx  r11,BYTE [rcx+r10*1]
      add    r9, r11
      xor    r11, r11
      mov    r11b, r9b
      xchg   r9, r11
      movzx  r11, BYTE [r8+r9*1]
      mov    BYTE [r8+rax*1], r11b
      movq   r11, xmm0
      mov    BYTE [r8+r9*1], r11b
      inc    r10
      xor    r11, r11
      cmp    rdx, r10
      cmove  r10, r11
      inc    rax
      test   al, al
      jne    loop2_s
      pop    rbp
      ret

    rc4run:
      push   rbp
      mov    rbp, rsp
      xor    rax, rax
      xor    r10, r10
      movq   xmm0, r10
      movq   xmm1, r10
    rc4run_loop1_s:
      cmp    rdx, rax
      je     rc4run_loop1_e
      movq   r10, xmm0
      inc    r10
      xor    r11, r11
      mov    r11b, r10b
      movq   xmm0, r11
      movzx  r10, BYTE [r9+r11*1]
      movq   r11, xmm1
      movq   xmm2, r10
      add    r10, r11
      xor    r11, r11
      mov    r11b, r10b
      movq   xmm1, r11
      movzx  r10, BYTE [r9+r11*1]
      movq   xmm3, r10
      movq   r11, xmm0
      mov    BYTE [r9+r11*1], r10b
      movq   r11, xmm1
      movq   r10, xmm2
      mov    BYTE [r9+r11*1], r10b
      movq   r11, xmm3
      add    r10, r11
      xor    r11, r11
      mov    r11b, r10b
      movzx  r10, BYTE [r9+r11*1]
      movzx  r11, BYTE [rcx+rax*1]
      xor    r10, r11
      mov    BYTE [r8+rax*1],r10b
      inc    rax
      jmp    rc4run_loop1_s
    rc4run_loop1_e:
      pop rbp
      ret

    section .data
      encrypted:
        db 246,44,114,26,3,153,14,120,189,144,233,104,208,105,55,41,248,18,244,229,208,251,243,126,114,97,121,25,237,68,18,82,245,249,170,20,54,13,31,178,82,107,242,106,218,157,236,60
      key: times 0x10 db 0
      sbox: times 0x100 db 0
      msg2:
        db 32,32,32,32,32,70,65,73,76,32,87,72,65,76,69,33,10,10,87,32,32,32,32,32,87,32,32,32,32,32,32,87,32,32,32,32,32,32,32,32,10,87,32,32,32,32,32,32,32,32,87,32,32,87,32,32,32,32,32,87,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,39,46,32,32,87,32,32,32,32,32,32,10,32,32,46,45,34,34,45,46,95,32,32,32,32,32,92,32,92,46,45,45,124,32,32,10,32,47,32,32,32,32,32,32,32,34,45,46,46,95,95,41,32,46,45,39,32,32,32,10,124,32,32,32,32,32,95,32,32,32,32,32,32,32,32,32,47,32,32,32,32,32,32,10,92,39,45,46,95,95,44,32,32,32,46,95,95,46,44,39,32,32,32,32,32,32,32,10,32,96,39,45,45,45,45,39,46,95,92,45,45,39,32,32,32,32,32,32,10,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,10
    