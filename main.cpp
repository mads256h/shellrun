#include <cstdint>
#include <cstdio>
#include <cstring>

#include <filesystem>
#include <fstream>
#include <iostream>

//#include "config.h"
#include "util.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <Windows.h>
#endif

void print_usage() { puts("USAGE\nShellCodeRunner.exe file"); }

int main(int argc, char **argv) {
  if (argc == 1 || argc > 2) {
    // report version
    //std::cout << argv[0] << " Version " << ShellCodeRunner_VERSION_MAJOR << "."
    //          << ShellCodeRunner_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
    return 1;
  }
  const auto fs = std::filesystem::current_path() / argv[1];
  const auto len = std::filesystem::file_size(fs);
  uint8_t *const data = new uint8_t[len];

  std::ifstream file(argv[1], std::ifstream::binary);

  file.read((char *)&data[0], len);
  DEBUGBREAK();

  file.close();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
  DWORD why_must_this_variable;
  BOOL ret = VirtualProtect(data, len, PAGE_EXECUTE_READWRITE,
                            &why_must_this_variable);

  if (!ret) {
    printf("VirtualProtect\n");
    return EXIT_FAILURE;
  }
  puts("Hello world!");
#endif

  DEBUGBREAK();

  ((void (*)(void))data)();

  return 0;
}