#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdint.h>
#include <stdio.h>

void print_help(const char *const program_name);

void die(const char *const msg);

struct arguments {
  bool clear;
  bool debug;
  bool is_stdin;
};

struct arguments parse_arguments(const int argc, char *const argv[]);

uint8_t *alloc_file(FILE *const file, size_t *const len);

#endif