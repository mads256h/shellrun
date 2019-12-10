#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "config.h"
#include "util.h"

#define BUF_SIZE 512

enum arguments {
  a_none = 0,
  a_debug = 1 << 0,
  a_clear = 1 << 1,
};

void print_help(char *program_name) {
  fprintf(stderr, "%s Version %d.%d\n", program_name, shellrun_VERSION_MAJOR,
          shellrun_VERSION_MINOR);
  // clang-format off
  fprintf(stderr, "Usage: %s [args] file\n", program_name);
  fprintf(stderr, "Arguments\n");
  fprintf(stderr, " -d OR --debug   Set a breakpoint just before shellcode executes\n");
  fprintf(stderr, " -c OR --clear   Clear all registers before running the shellcode\n");
  fprintf(stderr, " -h OR --help    To print this help message\n");
  // clang-format on
}

int main(int argc, char **argv) {
  if (argc == 1) {
    // report version
    print_help(argv[0]);
    return EXIT_FAILURE;
  }

  if (strcmp(argv[argc - 1], "-h") == 0 ||
      strcmp(argv[argc - 1], "--help") == 0) {
    print_help(argv[0]);
    return EXIT_FAILURE;
  }

  enum arguments args = a_none;

  for (int i = 1; i < argc - 1; i++) {
    if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
      args |= a_debug;
    } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--clear") == 0) {
      args |= a_clear;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_help(argv[0]);
      return 1;
    } else {
      fputs("Invalid argument", stderr);
      return EXIT_FAILURE;
    }
  }

  FILE *f =
      strcmp(argv[argc - 1], "-") == 0 ? stdin : fopen(argv[argc - 1], "rb");

  uint8_t *fileData = (uint8_t *)malloc(BUF_SIZE);

  if (!fileData) {
    fputs("Could not allocate fileData", stderr);
    return EXIT_FAILURE;
  }

  size_t cur_size = BUF_SIZE;
  size_t len = 0;

  int c = EOF;
  while ((c = getc(f)) != EOF) {
    if (cur_size == len) {
      cur_size += BUF_SIZE;
      uint8_t *const d = (uint8_t *)malloc(cur_size);

      if (!d) {
        fputs("Could not allocate d", stderr);
        return EXIT_FAILURE;
      }

      memcpy(d, fileData, len);
      free(fileData);
      fileData = d;
    }
    fileData[len] = (uint8_t)c;
    len++;
  }

  if (strcmp(argv[argc - 1], "-") != 0) {
    fclose(f);
  }

  size_t final_len = len;
  if ((args & a_debug) == a_debug) {
    final_len += sizeof(int3);
  }
  if ((args & a_clear) == a_clear) {
    final_len += sizeof(clear);
  }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
  uint8_t *const shellcode = (uint8_t *)malloc(final_len);
#else
  const long pagesize = sysconf(_SC_PAGE_SIZE);

  uint8_t *const shellcode = (uint8_t *)memalign(pagesize, final_len);
#endif

  if (!shellcode) {
    fputs("Could not allocate shellcode", stderr);
    return EXIT_FAILURE;
  }

  memset(shellcode, 0x90, final_len);

  size_t i = 0;

  if ((args & a_clear) == a_clear) {
    memcpy(shellcode, clear, sizeof(clear));
    i += sizeof(clear);
  }
  if ((args & a_debug) == a_debug) {
    memcpy(shellcode + i, int3, sizeof(int3));
    i += sizeof(int3);
  }

  memcpy(shellcode + i, fileData, len);

  free(fileData);

  MPROTECT(shellcode, len);

  ((void (*)(void))shellcode)();

  return EXIT_SUCCESS;
}