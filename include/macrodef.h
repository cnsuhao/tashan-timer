
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

#ifdef _WIN32

#ifndef _ULONGLONG_
typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;
typedef LONGLONG *PLONGLONG;
typedef ULONGLONG *PULONGLONG;
#endif // _ULONGLONG_

#else
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


