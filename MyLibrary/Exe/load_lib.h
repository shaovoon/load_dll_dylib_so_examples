#pragma once

#ifdef _MSC_VER

#include <windows.h>

#define DLL_HANDLE HMODULE
#define DLL_FUNC_PTR FARPROC

inline DLL_HANDLE LoadLib(const char* dll)
{
	return ::LoadLibraryA(dll);
}
inline BOOL FreeLib(DLL_HANDLE handle)
{
	return ::FreeLibrary(handle);
}
inline DLL_FUNC_PTR GetFunction(DLL_HANDLE handle, const char* funcName)
{
	return ::GetProcAddress(handle, funcName);
}
#else

#include <dlfcn.h>

#define DLL_HANDLE void*
#define DLL_FUNC_PTR void*

inline DLL_HANDLE LoadLib(const char* dll)
{
	return dlopen(dll, RTLD_LAZY);
}
inline int FreeLib(DLL_HANDLE handle)
{
	return dlclose(handle);
}
inline DLL_FUNC_PTR GetFunction(DLL_HANDLE handle, const char* funcName)
{
	return dlsym(handle, funcName);
}
#endif
