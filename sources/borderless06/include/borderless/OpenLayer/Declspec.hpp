#ifndef OL_DECLSPEC_HPP
#define OL_DECLSPEC_HPP

#if defined (_WIN32) && defined (OL_LIB_BUILD) && defined (OL_SHARED)
#define OL_LIB_DECLSPEC __declspec(dllexport)
#define OL_EXT_DECLSPEC __declspec(dllexport)
#elif defined (_WIN32) && defined (OL_SHARED)
#define OL_LIB_DECLSPEC __declspec(dllimport)
#define OL_EXT_DECLSPEC __declspec(dllimport)
#else
#define OL_LIB_DECLSPEC
#define OL_EXT_DECLSPEC
#endif


#endif // OL_DECLSPEC_HPP

