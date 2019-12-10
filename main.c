#include <errno.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <sys/stat.h>

#include "asm.h"
#include "config.h"
#include "util.h"

enum flags {
  f_none = 0,
  f_debug = 1 << 0,
  f_clear = 1 << 1,
};

void print_help(char *program_name) {
  printf("%s Version %d.%d\n", program_name, shellrun_VERSION_MAJOR,
         shellrun_VERSION_MINOR);
  printf("Usage: %s [args] file\n", program_name);
  puts("Arguments");
  puts(" -d OR --debug   Set a breakpoint just before shellcode executes");
  puts(" -c OR --clear   Clear all registers before running the shellcode");
  puts(" -h OR --help    To print this help message");
}

int main(int argc, char **argv) {
  if (argc == 1) {
    // report version
    print_help(argv[0]);

    return 1;
  }

  if (strcmp(argv[argc - 1], "-h") == 0 ||
      strcmp(argv[argc - 1], "--help") == 0) {
    print_help(argv[0]);
    return 1;
  }

  enum flags flag = f_none;

  for (int i = 1; i < argc - 1; i++) {
    if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
      flag |= f_debug;
    } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--clear") == 0) {
      flag |= f_clear;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_help(argv[0]);
      return 1;
    } else {
      fputs("Invalid argument", stderr);
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