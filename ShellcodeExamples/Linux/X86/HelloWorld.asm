SYS_WRITE equ 1
SYS_EXIT equ 60
STD_OUTPUT equ 1
 
section .text
global start
 
start:
call MainCode
	
 
MainCode:
    pop ebx
	mov eax, SYS_WRITE
	mov edi, STD_OUTPUT
	lea esi, [msg - MainCode + ebx]
	mov edx, msglen
    xor ebx,ebx
	int 0x80
 
	mov eax, SYS_EXIT
	mov edi, 0
	int 0x80

    msg: db "Hello World!\n"
	msglen equ $-msg