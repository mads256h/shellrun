BITS 64

extern GetStdHandle
extern WriteFile
extern ExitProcess

section .rodata

msg db "Hello World!", 0x0d, 0x0a

msg_len equ $-msg
stdout_query equ -11

section .data

stdout dw 0
bytes_written dw 0

section .text

global start

start:
    mov rcx, stdout_query
    call GetStdHandle
    mov [rel stdout], rax

    mov  rcx, [rel stdout]
    mov  rdx, msg
    mov  r8, msg_len
    mov  r9, bytes_written
    push qword 0
    call WriteFile

    xor rcx, rcx
    call ExitProcess