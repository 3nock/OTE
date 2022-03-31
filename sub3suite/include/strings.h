/*Dummy file to satisfy source file dependencies on Windows platform*/
#if defined(_WIN32)
	#define strcasecmp _stricmp
	#define strncasecmp _strnicmp
	#define inline __inline
#endif
