#pragma once // FIXME we actually need to re-evaluate NDEBUG each time

// assert.h C99 standard header

#ifndef NDEBUG
#define assert(x) proxylibc_assert(x)
void proxylibc_assert(int t);
#else
#define assert(x) ((void)0)
#endif

#define static_assert _Static_assert

