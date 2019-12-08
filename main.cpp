

#include <cstdio>
#include <cstring>
#include <cstdint>

#include <fstream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <Windows.h>
#endif

void print_usage(){
    puts("USAGE\nShellCodeRunner.exe file");
}

int main(int argc, char** argv){
    if (argc == 1 || argc > 2){
        print_usage();
        return 1;
    }

    std::ifstream file(argv[1]);
    file.seekg(0, std::ios::end);
    size_t len = file.tellg();
    uint8_t *data = new uint8_t[len];
    file.read((char*)&data[0], len);
    file.close();

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
    DWORD why_must_this_variable;
   BOOL ret = VirtualProtect (data, len,
    PAGE_EXECUTE_READWRITE, &why_must_this_variable);

  if (!ret) {
    printf ("VirtualProtect\n");
    return EXIT_FAILURE;
  }
    #endif

    int (*func)();
    func = (int(*)()) data;
    (int)(*func)();

    return 0;
}