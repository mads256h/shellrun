#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

#include "asm.h"
#include "config.h"
#include "util.h"

#define BUF_SIZE 512

int main(int argc, char *argv[]) {
  // Check if any arguments has been given
  if (argc == 1) {
    print_help(argv[0]);
    return EXIT_FAILURE;
  }

  // Print help if the last argument is help
  if (check_for_argument(argv[argc - 1], "h", "help")) {
    print_help(argv[0]);
    return EXIT_SUCCESS;
  }

  const struct arguments args = parse_arguments(argc, argv);

  FILE *file = 0;

  if (args.is_stdin) {
    file = stdin;
  } else {
    file = fopen(argv[argc - 1], "rb");

    if (!file)
      die("Could not open file");
  }

  size_t file_len = 0;

  uint8_t *const file_data = alloc_file(file, &file_len);

  if (!args.is_stdin)
    fclose(file);

  size_t final_len = file_len;

  if (args.clear) {
    final_len += sizeof(asm_clear);
  }

  if (args.debug) {
    final_len += sizeof(asm_int3);
  }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
  uint8_t *const shellcode = (uint8_t *)malloc(final_len);
#else
  const long pagesize = sysconf(_SC_PAGE_SIZE);
  uint8_t *const shellcode = (uint8_t *)memalign(pagesize, final_len);
#endif

  if (!shellcode)
    die("Could not allocate shellcode");

  size_t prefix_len = 0;

  if (args.clear) {
    memcpy(shellcode, asm_clear, sizeof(asm_clear));
    prefix_len += sizeof(asm_clear);
  }
  if (args.debug) {
    memcpy(shellcode + prefix_len, asm_int3, sizeof(asm_int3));
    prefix_len += sizeof(asm_int3);
  }

  memcpy(shellcode + prefix_len, file_data, file_len);

  free(file_data);

  MPROTECT(shellcode, final_len);

  ((void (*)(void))shellcode)();

  return EXIT_SUCCESS;
}

uint8_t *alloc_file(FILE *const file, size_t *const file_len) {
  uint8_t *file_data = (uint8_t *)malloc(BUF_SIZE);

  if (!file_data)
    die("Could not allocate file data");

  size_t cur_size = BUF_SIZE;
  *file_len = 0;

  int c = EOF;
  while ((c = getc(file)) != EOF) {
    if (cur_size == *file_len) {
      cur_size += BUF_SIZE;
      uint8_t *const new_file_data = (uint8_t *)malloc(cur_size);

      if (!new_file_data)
        die("Could not allocate new file data");

      memcpy(new_file_data, file_data, *file_len);
      free(file_data);
      file_data = new_file_data;
    }
    file_data[*file_len] = (uint8_t)c;
    *file_len += 1;
  }

  return file_data;
}

struct arguments parse_arguments(const int argc, char *const argv[]) {
  struct arguments args;

  args.clear = false;
  args.debug = false;
  // Is true when last argument is a dash: -
  args.is_stdin = strcmp(argv[argc - 1], "-") == 0;

  for (int i = 1; i < argc - 1; i++) {
    if (check_for_argument(argv[i], "c", "clear")) {
      args.clear = true;
    } else if (check_for_argument(argv[i], "d", "debug")) {
      args.debug = true;
    } else if (check_for_argument(argv[i], "h", "help")) {
      print_help(argv[0]);
      exit(EXIT_SUCCESS);
    } else {
      fputs("Invalid argument", stderr);
      exit(EXIT_FAILURE);
    }
  }

  return args;
}

// Prints version and help/usage
void print_help(const char *const program_name) {
  fprintf(stderr, "%s Version %d.%d\n", program_name, shellrun_VERSION_MAJOR,
          shellrun_VERSION_MINOR);

  // clang-format off
  fprintf(stderr, "Usage: %s [args] file\n", program_name);
  fprintf(stderr, "Arguments\n");
  fprintf(stderr, " -c OR --clear   Clear all registers before running the shellcode\n");
  fprintf(stderr, " -d OR --debug   Set a breakpoint just before shellcode executes\n");
  fprintf(stderr, " -h OR --help    To print this help message\n");
  // clang-format on
}

// Prints a system error with explanation
void die(const char *const msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}