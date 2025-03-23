#include <cstdio>
#ifdef _MSC_VER
#include "../MyLibrary/MyLibrary.h"
#else
#include "../../MyLibrary/MyLibrary.h"
#endif

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
