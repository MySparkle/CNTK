#ifndef __PLATFORM_H
#define __PLATFORM_H

#if defined( _MSC_VER )
#define __WINDOWS__
#elif defined( __GNUC__ )
#define __UNIX__
#endif

#ifdef __UNIX__
// necessary header files for API conversion
//#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <assert.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
// basic type conversion
typedef int BOOL;
typedef unsigned char BYTE;
typedef BYTE BOOLEAN;
typedef char CHAR;
typedef float FLOAT;
typedef int INT;
typedef long LONG;
typedef long long LONG64;
typedef short SHORT;
typedef unsigned char UCHAR;
typedef unsigned int UINT;
typedef wchar_t WCHAR;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef std::intptr_t INT_PTR;
typedef const char* LPCSTR;
typedef const wchar_t * LPCWSTR;
typedef unsigned long ULONG;
typedef int errno_t;
typedef char TCHAR;
typedef void* HANDLE;
#define interface class
#define WINAPI  
#define VOID void
#define CONST const

//standard library conversion
//#define min std::min
#define hash_map unordered_map

//macro conversion
#define __forceinline inline
//string and io conversion
#define strtok_s strtok_r
#define sprintf_s sprintf
#define sscanf_s sscanf
#define _strdup strdup

//file io API conversion
#define _FILE_OFFSET_BITS 64
inline errno_t _fopen_s(FILE **file, const char *fileName, const char *mode)
{
	FILE *f = fopen(fileName, mode);
	if (f == NULL)
		return -1;
	file = &f;
	return 0;
}

inline errno_t memcpy_s(void *dest, size_t numberOfElements, const void *src, size_t count)
{
	if (dest == NULL || src == NULL || numberOfElements < count)
		return -1;
	memcpy(dest, src, count);
	return 0;
}

inline int _fseeki64(FILE *file, int64_t offset, int origin)
{
	return fseeko(file, offset, origin);
}

inline int _ftelli64(FILE *file)
{
	return ftello(file);
}

inline long GetTickCount(void)
{
	struct timespec now;
	if (clock_gettime(CLOCK_MONOTONIC, &now))
		return 0;
	return now.tv_sec * 1000 + now.tv_nsec / 1000000;
}

inline errno_t strcpy_s(char *strDest, size_t numElem, const char *strSrc)
{
	assert(strDest != NULL && strSrc != NULL);
	strncpy(strDest, strSrc, numElem);
	return 0;
}

inline errno_t wcstombs_s(size_t *prt, char *mbStr, size_t sizeInBytes, const wchar_t *wcStr, size_t count)
{
	// no safety checking
	return ::wcstombs(mbStr, wcStr, count);
}

inline int fscanf_s(FILE *stream, const char *format, ...)
{
	va_list args;
	va_start(args, format); 
	int res = vfscanf(stream, format, args); 
	va_end(args);
	return res;
}

inline char* wtocharpath (const wchar_t *p)
{
    size_t len = std::basic_string<wchar_t>(p).length();
    char *buf = new char[2 * len + 1]; // max: 1 wchar => 2 mb chars
    ::wcstombs(buf, p, 2 * len + 1);
    return buf;
}

inline errno_t _wfopen_s(FILE **file, const wchar_t *fileName, const wchar_t *mode)
{
	char *fn = wtocharpath(fileName);
	char *m = wtocharpath(mode);
	FILE *f = fopen(fn, m);
	delete[] fn;
	delete[] m;
	if (f == NULL)
		return -1;
	*file = f;
	return 0;
}

inline pid_t GetCurrentProcessId()
{
	return getpid();
}

#endif //__UNIX__

#endif //__PLATFORM_H
