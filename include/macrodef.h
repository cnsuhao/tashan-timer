
#pragma once

#ifdef __LINUX__
#include <sys/types.h>
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef USE_VARIABLE
#define USE_VARIABLE(x) (void)(x)
#endif



#ifndef _WIN32

/* Define int32_t, int64_t, and uint64_t types for UST/MSC */
/* (as used in the GL_EXT_timer_query extension). */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <inttypes.h>
#elif defined(__sun__) || defined(__digital__)
#include <inttypes.h>
#if defined(__STDC__)
#if defined(__arch64__) || defined(_LP64)
typedef long int int64_t;
typedef unsigned long int uint64_t;
#else
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#endif /* __arch64__ */
#endif /* __STDC__ */
#elif defined( __VMS ) || defined(__sgi)
#include <inttypes.h>
#elif defined(__SCO__) || defined(__USLC__)
#include <stdint.h>
#elif defined(__UNIXOS2__) || defined(__SOL64__)
typedef long int int32_t;
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#elif defined(_WIN32) && defined(__GNUC__)
#include <stdint.h>
#elif defined(_WIN32)
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
/* Fallback if nothing above works */
#include <inttypes.h>
#endif

typedef int64_t __int64;
typedef int64_t __time64_t;
typedef long LONG;
#ifndef _ULONGLONG_
typedef __int64 LONGLONG;
//typedef unsigned __int64 ULONGLONG;
typedef u_int64_t ULONGLONG;
typedef LONGLONG *PLONGLONG;
typedef ULONGLONG *PULONGLONG;
#endif // _ULONGLONG_

typedef void *HINSTANCE;
typedef int64_t LONGLONG;
typedef u_int64_t ULONGLONG;
typedef LONGLONG *PLONGLONG;
typedef ULONGLONG *PULONGLONG;

typedef void *HANDLE;
typedef void *HMODULE;
typedef void *HINSTANCE;
typedef  int INT_PTR, *PINT_PTR;
typedef  unsigned int UINT_PTR, *PUINT_PTR;

typedef  long LONG_PTR, *PLONG_PTR;
typedef  unsigned long ULONG_PTR, *PULONG_PTR;

#ifndef near
#define near
#endif

#ifndef far
#define far
#endif

#ifndef CONST
#define CONST               const
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#if defined(DEBUG) || defined(_DEBUG)
#ifndef fl_assert
#define fl_assert(__x) assert(__x)
#endif
#else
#ifndef fl_assert
#define fl_assert(__x) 
#endif
#endif

#ifndef min 
#define min(x,y) ({ typeof(x) _x = (x); typeof(y) _y = (y); (void) (&_x == &_y); _x < _y ? _x : _y; })
#endif

#ifndef max
#define max(x,y) ({ typeof(x) _x = (x); typeof(y) _y = (y); (void) (&_x == &_y); _x > _y ? _x : _y; })
#endif
 
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL near           *PBOOL;
typedef BOOL far            *LPBOOL;
typedef BYTE near           *PBYTE;
typedef BYTE far            *LPBYTE;
typedef int near            *PINT;
typedef int far             *LPINT;
typedef WORD near           *PWORD;
typedef WORD far            *LPWORD;
typedef long far            *LPLONG;
typedef DWORD near          *PDWORD;
typedef DWORD far           *LPDWORD;
typedef void far            *LPVOID;
typedef CONST void far      *LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

#define CONTAINING_RECORD(address, type, field) ((type *)( \
                                                  (unsigned char*)(address) - \
                                                  (ULONG_PTR)(&((type *)0)->field)))

#endif


