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
