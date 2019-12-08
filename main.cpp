#include <cstdint>
#include <cstdio>
#include <cstring>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.h"
#include "util.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <Windows.h>
#endif

int main(int argc, char **argv) {
  if (argc == 1 || argc > 2) {
    // report version
    std::cout << argv[0] << " Version " << ShellCodeRunner_VERSION_MAJOR << "."
              << ShellCodeRunner_VERSION_MINOR << std::endl
              << "Usage: " << argv[0] << " file" << std::endl;
    return 1;
  }
  const auto fs = std::filesystem::current_path() / argv[1];
  const auto len = std::filesystem::file_size(fs);
  uint8_t *const data = new uint8_t[len];

  std::ifstream file(argv[1], std::ifstream::binary);

  file.read((char *)&data[0], len);

  file.close();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
  DWORD previous_access;
  BOOL ret =
      VirtualProtect(data, len, PAGE_EXECUTE_READWRITE, &previous_access);

  if (!ret) {
    printf("VirtualProtect\n");
    return EXIT_FAILURE;
  }
#endif

  DEBUGBREAK();

  ((void (*)(void))data)();

  return 0;
}