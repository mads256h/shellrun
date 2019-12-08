#ifdef _MSC_VER
#include <intrins.h>
#define DEBUGBREAK() __debugbreak()
#endif
#ifdef __GNUC__
#define DEBUGBREAK() asm("int3")
#endif