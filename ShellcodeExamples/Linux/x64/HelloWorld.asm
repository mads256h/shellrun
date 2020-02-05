BITS 64

SYS_WRITE equ 1
SYS_EXIT equ 60
STD_OUTPUT equ 1

section .text
global start

start:
    call MainCode

MainCode:
    ; Print hello world to stdout
    pop rsi ; Get rip into rsi
    mov rax, SYS_WRITE
    mov rdi, STD_OUTPUT
    lea rsi, [msg - MainCode + rsi] ; Load msg by using relative indexing.
    mov rdx, msglen ; msglen is just a constant so no relative indexing is needed.
    syscall

    ; Exit with return value 0
    mov rax, SYS_EXIT
    mov rdi, 0 ; return value
    syscall

    msg: db `Hello world!\n`
    msglen equ $-msg