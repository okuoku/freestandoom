#pragma once
// stdlib.h C99 standard header

// FIXME:
#include <stddef.h> // NULL, size_t

// div_t
// ldiv_t
// lldiv_t
#define EXIT_FAILURE proxylibc_EXIT_FAILURE
#define proxylibc_EXIT_FAILURE 1
#define EXIT_SUCCESS proxylibc_EXIT_SUCCESS
#define proxylibc_EXIT_SUCCESS 0
// RAND_MAX
// MB_CUR_MAX

#define atof proxylibc_atof
double atof(const char *nptr);
#define atoi proxylibc_atoi
int atoi(const char *nptr);

// atol
// atoll
// strtod
// strtof
// strtold
// strtol
// strtoll
// strtoul
// strtoull

// rand
// srand

// aligned_alloc

#define calloc proxylibc_calloc
void *calloc(size_t nmemb, size_t size);

#define free proxylibc_free
void free(void* ptr);

#define malloc proxylibc_malloc
void *malloc(size_t size);

#define realloc proxylibc_realloc
void *realloc(void *ptr, size_t size);

// abort
// atexit
// at_quick_exit
#define exit proxylibc_exit
_Noreturn void exit(int status);
// _Exit
// getenv
// quick_exit

#define system proxylibc_system
int system(const char *string);

// bsearch
// qsort

#define abs proxylibc_abs
int abs(int j);
// labs
// llabs

// div
// ldiv
// lldiv

// mblen
// mbtowc
// wctomb
// mbstowcs
// wcstombs

