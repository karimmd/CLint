#ifndef configH
#define configH

#ifdef _WIN32
#  ifdef CLINTLIB_EXPORT
#    define CLINTLIB __declspec(dllexport)
#  elif defined(CLINTLIB_IMPORT)
#    define CLINTLIB __declspec(dllimport)
#  else
#    define CLINTLIB
#  endif
#else
#  define CLINTLIB
#endif

// MS Visual C++ memory leak debug tracing
#if defined(_MSC_VER) && defined(_DEBUG)
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#endif

#include <string>
static const std::string emptyString;

#endif // configH
