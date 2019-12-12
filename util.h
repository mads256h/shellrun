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
      LPSTR messageBuffer = 0;                                                 \
      size_t size = FormatMessageA(                                            \
          FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |        \
              FORMAT_MESSAGE_IGNORE_INSERTS,                                   \
          NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),     \
          (LPSTR)&messageBuffer, 0, NULL);                                     \
                                                                               \
      printf("VirtualProtect error: %s\n", messageBuffer);                     \
      LocalFree(messageBuffer);                                                \
      return EXIT_FAILURE;                                                     \
    }                                                                          \
  }
#else
#include <malloc.h>
#include <sys/mman.h>
#include <unistd.h>
#define MPROTECT(shellcode, length)                                            \
  {                                                                            \
    int res = mprotect(shellcode, length, PROT_EXEC | PROT_READ | PROT_WRITE); \
    if (res) {                                                                 \
      perror("mprotect error");                                                \
      return 1;                                                                \
    }                                                                          \
  }
#endif

#endif