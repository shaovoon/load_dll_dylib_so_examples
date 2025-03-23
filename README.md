## Loading dynamic library of Windows (.dll), MacOS (.dylib) and Unix/Linux (.so)

Windows use `LoadLibrary` to load dll dynamically at run time while MacOS, Unix and Linux use the same `dlopen`. To get function pointers, Windows makes use of `GetProcAddress` while MacOS, Unix and Linux equivalent is `dlsym`. To unload the dynamic library on Windows, call `FreeLibrary` while on MacOS, Unix and Linux call `dlclose` though unloading is rarely done. I define three functions namely `LoadLib`, `GetFunction` and `FreeLib` to abstract away the platform differences.

```Cpp
#ifdef _MSC_VER

#include <windows.h>

#define DLL_HANDLE HMODULE
#define DLL_FUNC_PTR FARPROC

DLL_HANDLE LoadLib(const char* dll)
{
    return ::LoadLibraryA(dll);
}
BOOL FreeLib(DLL_HANDLE handle)
{
    return ::FreeLibrary(handle);
}
DLL_FUNC_PTR GetFunction(DLL_HANDLE handle, const char* funcName)
{
    return ::GetProcAddress(handle, funcName);
}
#else

#include <dlfcn.h>

#define DLL_HANDLE void*
#define DLL_FUNC_PTR void*

DLL_HANDLE LoadLib(const char* dll)
{
    return dlopen(dll, RTLD_LAZY);
}
int FreeLib(DLL_HANDLE handle)
{
    return dlclose(handle);
}
DLL_FUNC_PTR GetFunction(DLL_HANDLE handle, const char* funcName)
{
    return dlsym(handle, funcName);
}
#endif
```

The example to load and get function address of `bar` as shown below. I use C++11 `decltype` to declare the `bar` function type because I want to avoid declaring the function pointer type myself manually.

```Cpp
int main()
{
#ifdef _MSC_VER
    const char* libName = "MyLibrary.dll";
#elif defined(__APPLE__)
    const char* libName = "libMyLibrary.dylib";
#endif

    DLL_HANDLE handle = LoadLib(libName);

    if (handle == NULL)
    {
        fprintf(stderr, "Loading %s fails\n", libName);
        return 1;
    }

    decltype(bar)* fun_ptr = (decltype(bar)*)(GetFunction(handle, "bar"));

    printf("bar : %d\n", fun_ptr());

    return 0;
}
```

The `printf` output is as below when `LoadLib` returns a non-null valid handle.

```
bar : 2
```

The `bar` is declared in `MyLibrary.h` as below. If C++ source code includes this header, the function needs to be declared `extern "C"` to prevent the C++ mangling its name, so getting function address using its unmangled/original name can work.

```Cpp
#ifdef _MSC_VER
    #ifdef MYLIBRARY_EXPORTS
        #define MYLIBRARY_API __declspec(dllexport)
    #else
        #define MYLIBRARY_API __declspec(dllimport)
    #endif
#endif

#ifdef __APPLE__
    #define MYLIBRARY_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

MYLIBRARY_API int bar(void);


#ifdef __cplusplus
}
#endif
```

`bar` body is defined in `MyLibrary.cpp`. Same here, `extern "C"` has to be applied here.

```Cpp
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
```

**Note**: I have not tested it on Linux yet as I was locked out of my WSL as I have forgotten my password. Even uninstalling and reinstalling WSL, it retains my forgotten password.