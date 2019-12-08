#ifdef _MSC_VER
#define DEBUGBREAK() __asm {int 3}
#endif
#ifdef __GNUC__
#define DEBUGBREAK() asm("int3")
#endif