#include <cstdint>
#include <cstdio>
#include <cstring>
#include <malloc.h>

#include <sys/stat.h>

#include <fstream>
#include <iostream>

#include "asm.h"
#include "config.h"
#include "util.h"

enum class flags {
  debug = 1 << 0,
  clear = 1 << 1,
};

int main(int argc, char **argv) {
  if (argc == 1) {
    // report version
    std::cout << argv[0] << " Version " << ShellCodeRunner_VERSION_MAJOR << "."
              << ShellCodeRunner_VERSION_MINOR << std::endl
              << "Usage: " << argv[0] << "[flags] file" << std::endl;
    return 1;
  }

  flags flag;

  for (int i = 1; i < argc - 1; i++) {
    if (strcmp(argv[i], "--debug") == 0) {
      flag = (::flags)((int)flag | (int)flags::debug);
    } else if (strcmp(argv[i], "--clear") == 0) {
      flag = (::flags)((int)flag | (int)flags::clear);
    } else {
      std::cerr << "Invalid argument" << std::endl;
    }
  }

  struct stat stat_buf;
  const auto rc = stat(argv[argc - 1], &stat_buf);
  const auto len = rc == 0 ? stat_buf.st_size : -1;

  uint8_t *const fileData = new uint8_t[len];

  std::ifstream file(argv[argc - 1], std::ifstream::binary);
  file.read((char *)&fileData[0], len);
  file.close();

  auto final_len = len;
  if (((int)flag & (int)::flags::debug) == (int)flags::debug) {
    final_len += sizeof(int3);
  }
  if (((int)flag & (int)::flags::clear) == (int)flags::clear) {
    final_len += sizeof(clear);
  }
  uint8_t *const shellcode = new uint8_t[final_len];

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
  uint8_t *const shellcode = new uint8_t[final_len];
#else
  const auto pagesize = sysconf(_SC_PAGE_SIZE);

  uint8_t *const shellcode =
      static_cast<uint8_t *>(memalign(pagesize, final_len));
#endif

  size_t i = 0;

  if (((int)flag & (int)::flags::clear) == (int)flags::clear) {
    i += sizeof(clear);
    memcpy(shellcode, clear, sizeof(clear));
  }
  if (((int)flag & (int)::flags::debug) == (int)flags::debug) {
    i += sizeof(int3);
    memcpy(shellcode + i, int3, sizeof(int3));
  }

  memcpy(shellcode + i, fileData, len);

  MPROTECT(shellcode, len);

  ((void (*)(void))shellcode)();

  return 0;
}