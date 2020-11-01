#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define check_for_argument(argv, short, long)                                  \
  (strcmp(argv, "-" short) == 0 || strcmp(argv, "--" long) == 0)

#ifdef WINDOWS
#include <Windows.h>
#define MPROTECT(shellcode, length)                                            \
  {                                                                            \
    DWORD previous_access;                                                     \
    const BOOL ret = VirtualProtect(shellcode, length, PAGE_EXECUTE_READWRITE, \
                                    &previous_access);                         \
                                                                               \
    if (!ret) {                                                                \
      LPSTR messageBuffer = 0;                                                 \
      const size_t size = FormatMessageA(                                      \
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
    const int res =                                                            \
        mprotect(shellcode, length, PROT_EXEC | PROT_READ | PROT_WRITE);       \
    if (res) {                                                                 \
      perror("mprotect error");                                                \
      return EXIT_FAILURE;                                                     \
    }                                                                          \
  }
#endif

#endif
