#ifndef _ASM_H_
#define _ASM_H_

#include <cstdint>

#include "util.h"

const uint8_t int3[] = {0x33_u8};

#ifdef ENVIRONMENT64
// 64-bit version
const uint8_t clear[] = {
    // Clear all registers
    0x48_u8, 0x31_u8, 0xC0_u8, // xor    rax,rax
    0x48_u8, 0x31_u8, 0xDB_u8, // xor    rbx,rbx
    0x48_u8, 0x31_u8, 0xC9_u8, // xor    rcx,rcx
    0x48_u8, 0x31_u8, 0xD2_u8, // xor    rdx,rdx
    0x48_u8, 0x31_u8, 0xF6_u8, // xor    rsi,rsi
    0x48_u8, 0x31_u8, 0xFF_u8, // xor    rdi,rdi
    0x4D_u8, 0x31_u8, 0xC0_u8, // xor    r8,r8
    0x4D_u8, 0x31_u8, 0xC9_u8, // xor    r9,r9
    0x4D_u8, 0x31_u8, 0xD2_u8, // xor    r10,r10
    0x4D_u8, 0x31_u8, 0xDB_u8, // xor    r11,r11
    0x4D_u8, 0x31_u8, 0xE4_u8, // xor    r12,r12
    0x4D_u8, 0x31_u8, 0xED_u8, // xor    r13,r13
    0x4D_u8, 0x31_u8, 0xF6_u8, // xor    r14,r14
    0x4D_u8, 0x31_u8, 0xFF_u8, // xor    r15,r15
    // Reset stack
    0x48_u8, 0x89_u8, 0xEC_u8 // mov    rsp,rbp
};
#else
// 32-bit version
const uint8_t clear[] = {
    // Clear all registers
    0x31_u8, 0xC0_u8, // xor    eax,eax
    0x31_u8, 0xDB_u8, // xor    ebx,ebx
    0x31_u8, 0xC9_u8, // xor    ecx,ecx
    0x31_u8, 0xD2_u8, // xor    edx,edx
    0x31_u8, 0xF6_u8, // xor    esi,esi
    0x31_u8, 0xFF_u8, // xor    edi,edi
    // Reset stack
    0x89_u8, 0xEC_u8, // mov    esp,ebp
    0x33_u8};
#endif

#endif