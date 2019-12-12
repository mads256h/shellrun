SYS_WRITE equ 1
SYS_EXIT equ 60
STD_OUTPUT equ 1

section .text
global start

start:
    call MainCode
	
 
MainCode:
    pop rsi
    mov rax, SYS_WRITE
    mov rdi, STD_OUTPUT
    lea rsi, [msg - MainCode + rsi]
    mov rdx, msglen
    syscall

    mov rax, SYS_EXIT
    mov rdi, 0
    syscall

    msg: db `Hello world!\n`
    msglen equ $-msg