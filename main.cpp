#include <cstdint>
#include <cstdio>
#include <cstring>
#include <malloc.h>

#include <sys/stat.h>

#include <fstream>

#include "asm.h"
#include "config.h"
#include "util.h"

enum class flags {
  none = 0,
  debug = 1 << 0,
  clear = 1 << 1,
};

int main(int argc, char **argv) {
  if (argc == 1) {
    // report version
    printf("%s Version %d.%d\n", argv[0], ShellCodeRunner_VERSION_MAJOR, ShellCodeRunner_VERSION_MINOR);
    printf("Usage: %s [args] file\n", argv[0]);
    puts("Flags\n");
    puts(" --debug   Set a breakpoint just before shellcode executes\n");
    puts(" --clear   Clear all registers before running the shellcode\n");
    
    return 1;
  }

  flags flag = flags::none;

  for (int i = 1; i < argc - 1; i++) {
    if (strcmp(argv[i], "--debug") == 0) {
      flag = (::flags)((int)flag | (int)flags::debug);
    } else if (strcmp(argv[i], "--clear") == 0) {
      flag = (::flags)((int)flag | (int)flags::clear);
    } else {
      fprintf(stderr, "Invalid argument\n");
      return 1;
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

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
  uint8_t *const shellcode = new uint8_t[final_len];
#else
  const auto pagesize = sysconf(_SC_PAGE_SIZE);

  uint8_t *const shellcode =
      static_cast<uint8_t *>(memalign(pagesize, final_len));
#endif

  memset(shellcode, 0x90, final_len);

  size_t i = 0;

  if (((int)flag & (int)::flags::clear) == (int)flags::clear) {
    memcpy(shellcode, clear, sizeof(clear));
    i += sizeof(clear);
  }
  if (((int)flag & (int)::flags::debug) == (int)flags::debug) {
    memcpy(shellcode + i, int3, sizeof(int3));
    i += sizeof(int3);
  }

  memcpy(shellcode + i, fileData, len);

  MPROTECT(shellcode, len);

  ((void (*)(void))shellcode)();

  return 0;
}