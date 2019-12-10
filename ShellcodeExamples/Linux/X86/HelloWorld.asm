SYS_WRITE equ 4
SYS_EXIT equ 1
STD_OUTPUT equ 1
 
section .text
global start
 
start:
call MainCode
	
 
MainCode:
    pop esi
	mov eax, SYS_WRITE
	mov ebx, STD_OUTPUT
	lea ecx, [msg - MainCode + esi]
	mov edx, msglen
	int 0x80
 
	mov eax, SYS_EXIT
	mov ebx, 0
	int 0x80

    msg: db "Hello World!\n"
	msglen equ $-msg