#include <cstdint>
#include <cstdio>
#include <cstring>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.h"
#include "util.h"
#include "asm.h"

int main(int argc, char **argv) {
  if (argc == 1) {
    // report version
    std::cout << argv[0] << " Version " << ShellCodeRunner_VERSION_MAJOR << "."
              << ShellCodeRunner_VERSION_MINOR << std::endl
              << "Usage: " << argv[0] << "[flags] file" << std::endl;
    return 1;
  }

  const auto fs = std::filesystem::current_path() / argv[argc - 1];
  const auto len = std::filesystem::file_size(fs);
  uint8_t *const fileData = new uint8_t[len];

  std::ifstream file(argv[1], std::ifstream::binary);
  file.read((char *)&fileData[0], len);
  file.close();

  MPROTECT(fileData, len);

  DEBUGBREAK();

  ((void (*)(void))fileData)();

  return 0;
}