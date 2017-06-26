// needed by xml parser
//#define MARKUP_WCHAR

#ifdef GLIB_EXPORTS
#define GLIB_API __declspec(dllexport)
#elif GLIB_STATIC
#define GLIB_API extern
#else
#define GLIB_API __declspec(dllimport)
#endif

#ifdef GLIB_EXPORTS
#define GLIB_CLASS __declspec(dllexport)
#elif GLIB_STATIC
#define GLIB_CLASS
#else
#define GLIB_CLASS __declspec(dllimport)
#endif
