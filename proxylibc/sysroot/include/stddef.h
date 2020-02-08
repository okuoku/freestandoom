#pragma once
// stddef.h: C99 standard header

// FIXME: 
#include <stdint.h>

typedef int32_t ptrdiff_t;
typedef int32_t size_t;
typedef int32_t wchar_t;

#define NULL (0)
#define offsetof(x,y) __builtin_offsetof(x,y)

