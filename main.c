#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/stat.h>

#include "asm.h"
#include "config.h"
#include "util.h"

enum flags {
  f_none = 0,
  f_debug = 1 << 0,
  f_clear = 1 << 1,
};

int main(int argc, char **argv) {
  if (argc == 1) {
    // report version
    printf("%s Version %d.%d\n", argv[0], ShellCodeRunner_VERSION_MAJOR,
           ShellCodeRunner_VERSION_MINOR);
    printf("Usage: %s [args] file\n", argv[0]);
    puts("Flags");
    puts(" --debug   Set a breakpoint just before shellcode executes");
    puts(" --clear   Clear all registers before running the shellcode");

    return 1;
  }

  enum flags flag = f_none;

  for (int i = 1; i < argc - 1; i++) {
    if (strcmp(argv[i], "--debug") == 0) {
      flag = flag | f_debug;
    } else if (strcmp(argv[i], "--clear") == 0) {
      flag = flag | f_clear;
    } else {
      fprintf(stderr, "Invalid argument\n");
      return 1;
    }
  }

  struct stat stat_buf;
  const int rc = stat(argv[argc - 1], &stat_buf);
  const long len = rc == 0 ? stat_buf.st_size : -1;

  uint8_t *const fileData = (uint8_t *)malloc(len);

  FILE *f = fopen(argv[argc - 1], "rb");

  fread(&fileData[0], 1, len, f);
  fclose(f);

  long final_len = len;
  if ((flag & f_debug) == f_debug) {
    final_len += sizeof(int3);
  }
  if ((flag & f_clear) == f_clear) {
    final_len += sizeof(clear);
  }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
  uint8_t *const shellcode = (uint8_t *)malloc(final_len);
#else
  const long pagesize = sysconf(_SC_PAGE_SIZE);

  uint8_t *const shellcode = (uint8_t *)memalign(pagesize, final_len);
#endif

  memset(shellcode, 0x90, final_len);

  size_t i = 0;

  if ((flag & f_clear) == f_clear) {
    memcpy(shellcode, clear, sizeof(clear));
    i += sizeof(clear);
  }
  if ((flag & f_debug) == f_debug) {
    memcpy(shellcode + i, int3, sizeof(int3));
    i += sizeof(int3);
  }

  memcpy(shellcode + i, fileData, len);

  MPROTECT(shellcode, len);

  ((void (*)(void))shellcode)();

  return 0;
}