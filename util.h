#ifndef _UTIL_H_
#define _UTIL_H_
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
    long pagesize = sysconf(_SC_PAGE_SIZE);                                    \
    long page_no = (long)shellcode / pagesize;                                 \
    int res = mprotect((void *)(page_no * pagesize), (long)page_no + length,   \
                       PROT_EXEC | PROT_READ | PROT_WRITE);                    \
    if (res) {                                                                 \
      fprintf(stderr, "mprotect error:%d\n", res);                             \
      return 1;                                                                \
    }                                                                          \
  }
#endif

#endif