#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>

#ifdef _MSC_VER
#define DEBUGBREAK() __asm {int 3}
#endif
#ifdef __GNUC__
#define DEBUGBREAK() asm("int3")
#endif

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <Windows.h>
#define MPROTECT(shellcode, length)                                            \
  {                                                                            \
    DWORD previous_access;                                                     \
    BOOL ret = VirtualProtect(shellcode, length, PAGE_EXECUTE_READWRITE,       \
                              &previous_access);                               \
                                                                               \
    if (!ret) {                                                                \
      printf("VirtualProtect\n");                                              \
      return EXIT_FAILURE;                                                     \
    }                                                                          \
  }
#else
#include <sys/mman.h>
#include <unistd.h>
#define MPROTECT(shellcode, length)                                            \
  {                                                                            \
    int res =                                                                  \
        mprotect(shellcode, pagesize, PROT_EXEC | PROT_READ | PROT_WRITE);     \
    if (res) {                                                                 \
      fprintf(stderr, "mprotect error:%d\n", res);                             \
      fprintf(stderr, "errno:%d\n", errno);                                    \
      return 1;                                                                \
    }                                                                          \
  }
#endif

#endif