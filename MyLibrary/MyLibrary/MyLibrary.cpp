
#include "MyLibrary.h"

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C 
#endif

// This is an example of an exported function.
EXTERN_C MYLIBRARY_API int bar(void)
{
    return 2;
}
