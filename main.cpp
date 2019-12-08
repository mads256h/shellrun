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

int main(int argc, char **argv) {
  if (argc == 1) {
    // report version
    std::cout << argv[0] << " Version " << ShellCodeRunner_VERSION_MAJOR << "."
              << ShellCodeRunner_VERSION_MINOR << std::endl
              << "Usage: " << argv[0] << "[flags] file" << std::endl;
    return 1;
  }

  struct stat stat_buf;
  const auto rc = stat(argv[argc - 1], &stat_buf);
  const auto len = rc == 0 ? stat_buf.st_size : -1;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
  uint8_t *const fileData = new uint8_t[len];
#else

  auto pagesize = sysconf(_SC_PAGE_SIZE);

  uint8_t *const fileData =
      static_cast<uint8_t *>(memalign(pagesize, 4 * pagesize));
#endif

  std::ifstream file(argv[argc - 1], std::ifstream::binary);
  file.read((char *)&fileData[0], len);
  file.close();

  MPROTECT(fileData, len);

  DEBUGBREAK();

  ((void (*)(void))fileData)();

  return 0;
}