SYS_WRITE equ 4
SYS_EXIT equ 1
STD_OUTPUT equ 1
 
section .text
global start
 
start:
    call MainCode
	
 
MainCode:
    ; Print hello world to stdout
    pop esi ; Get eip into esi
	mov eax, SYS_WRITE
	mov ebx, STD_OUTPUT
	lea ecx, [msg - MainCode + esi] ; Load msg by using relative indexing.
	mov edx, msglen ; msglen is just a constant so no relative indexing is needed.
	int 0x80
 
    ; Exit with return value 0
	mov eax, SYS_EXIT
	mov ebx, 0 ; return value
	int 0x80

    msg: db `Hello World!\n`
	msglen equ $-msg