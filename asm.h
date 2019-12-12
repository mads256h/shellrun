#ifndef _ASM_H_
#define _ASM_H_

#include <stdint.h>

#include "util.h"

// value for the instruction int3
const uint8_t asm_int3[] = {'\xCC'};

#ifdef ENVIRONMENT64
// 64-bit version
const uint8_t asm_clear[] = {
    // Clear all registers
    '\x48', '\x31', '\xC0', // xor    rax,rax
    '\x48', '\x31', '\xDB', // xor    rbx,rbx
    '\x48', '\x31', '\xC9', // xor    rcx,rcx
    '\x48', '\x31', '\xD2', // xor    rdx,rdx
    '\x48', '\x31', '\xF6', // xor    rsi,rsi
    '\x48', '\x31', '\xFF', // xor    rdi,rdi
    '\x4D', '\x31', '\xC0', // xor    r8,r8
    '\x4D', '\x31', '\xC9', // xor    r9,r9
    '\x4D', '\x31', '\xD2', // xor    r10,r10
    '\x4D', '\x31', '\xDB', // xor    r11,r11
    '\x4D', '\x31', '\xE4', // xor    r12,r12
    '\x4D', '\x31', '\xED', // xor    r13,r13
    '\x4D', '\x31', '\xF6', // xor    r14,r14
    '\x4D', '\x31', '\xFF', // xor    r15,r15
    // Reset stack
    '\x48', '\x89', '\xEC' // mov    rsp,rbp
};
#else
// 32-bit version
const uint8_t asm_clear[] = {
    // Clear all registers
    '\x31', '\xC0', // xor    eax,eax
    '\x31', '\xDB', // xor    ebx,ebx
    '\x31', '\xC9', // xor    ecx,ecx
    '\x31', '\xD2', // xor    edx,edx
    '\x31', '\xF6', // xor    esi,esi
    '\x31', '\xFF', // xor    edi,edi
    // Reset stack
    '\x89', '\xEC', // mov    esp,ebp
};
#endif

#endif