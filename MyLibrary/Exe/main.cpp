#include <cstdio>
#ifdef _MSC_VER
#include "../MyLibrary/MyLibrary.h"
#else
#include "../../MyLibrary/MyLibrary.h"
#endif

#include "load_lib.h"

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
