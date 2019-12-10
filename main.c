#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "config.h"
#include "util.h"

#define BUF_SIZE 512

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

  bool a_debug = false;
  bool a_clear = false;

  for (int i = 1; i < argc - 1; i++) {
    if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
      a_debug = true;
    } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--clear") == 0) {
      a_clear = true;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_help(argv[0]);
      return 1;
    } else {
      fputs("Invalid argument", stderr);
      return EXIT_FAILURE;
    }
  }

  FILE *f = 0;

  if (strcmp(argv[argc - 1], "-") == 0) {
    f = stdin;
  } else {
    f = fopen(argv[argc - 1], "rb");

    if (!f) {
      perror("Could not open file");
      return EXIT_FAILURE;
    }
  }

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
      uint8_t *const newFileData = (uint8_t *)malloc(cur_size);

      if (!newFileData) {
        fputs("Could not allocate newFileData", stderr);
        return EXIT_FAILURE;
      }

      memcpy(newFileData, fileData, len);
      free(fileData);
      fileData = newFileData;
    }
    fileData[len] = (uint8_t)c;
    len++;
  }

  if (strcmp(argv[argc - 1], "-") != 0) {
    fclose(f);
  }

  size_t final_len = len;
  if (a_debug) {
    final_len += sizeof(asm_int3);
  }
  if (a_clear) {
    final_len += sizeof(asm_clear);
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

  if (a_clear) {
    memcpy(shellcode, asm_clear, sizeof(asm_clear));
    i += sizeof(asm_clear);
  }
  if (a_debug) {
    memcpy(shellcode + i, asm_int3, sizeof(asm_int3));
    i += sizeof(asm_int3);
  }

  memcpy(shellcode + i, fileData, len);

  free(fileData);

  MPROTECT(shellcode, len);

  ((void (*)(void))shellcode)();

  return EXIT_SUCCESS;
}