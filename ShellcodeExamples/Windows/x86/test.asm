PEBOffset equ 0x30
LoaderDataOffset equ 0x0C
InInitializationOrderModuleListOffset equ 0x1C
ImageBaseOffset equ 0x08
e_lfanew equ 0x3C
OptionalHeader equ 0x18
;Subsystem equ 0x44
DataDirectory0 equ 0x60
VirtualAddress equ 0x00
AddressOfFunctions equ 0x1C
AddressOfNameOrdinals equ 0x24
AddressOfNames equ 0x20
NumberOfNames equ 0x18

section .text
global start

start:
; Store eip in ebx to get our position in the executable
call get_delta
get_delta:

; Get Kernel32.dll
; PEB->LoaderData->InInitializationOrderModuleList
mov eax, [fs:PEBOffset] ; load addr of the PEB into eax
mov eax, [eax+LoaderDataOffset] ; load addr of LoaderData into eax
mov ebx, [eax+InInitializationOrderModuleListOffset] ; load addr of InInitializationOrderModuleList into ebx
mov ebx, [ebx] ; Follow List to get to kernel32.dll
mov esi, [ebx+ImageBaseOffset] ; Get image offset
mov edi, esi
mov eax, [esi+e_lfanew] ; Get offset to MZ header
add esi, eax ; apply offset now we have a IMAGE_NT_HEADER
;xor eax, eax
;mov ax, [esi+OptionalHeader+Subsystem] ; Get subsystem
Get_Export_Table:
mov eax, [esi+OptionalHeader+DataDirectory0+VirtualAddress]
add eax, edi
mov esi, eax
Get_Arrays:
mov eax, [esi+AddressOfFunctions]
add eax, edi
pop ebx
push ebx
lea ebx, [AddressFunctions - get_delta + ebx]
mov [ebx], eax
pop ebx
mov eax, [esi+AddressOfNameOrdinals]
add eax, edi
push ebx
lea ebx, [AddressNameOrdinals - get_delta + ebx]
mov [ebx], eax
pop ebx
mov eax, [esi+AddressOfNames]
add eax, edi
push ebx
lea ebx, [AddressNames - get_delta + ebx]
mov [ebx], eax
pop ebx
mov eax, [esi+NumberOfNames]
add eax, edi
push ebx
lea ebx, [NumberNames - get_delta + ebx]
mov [ebx], eax
pop ebx

nop
ret

AddressFunctions dd 0
AddressNameOrdinals dd 0
AddressNames dd 0
NumberNames dd 0