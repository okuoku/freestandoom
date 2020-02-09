#pragma once // FIXME we actually need to re-evaluate NDEBUG each time

// assert.h C99 standard header

#ifndef NDEBUG
#define assert(x) proxylibc_assert(x)
void proxylibc_assert_debug(const char* file, int line, int col, const char* func);
#define proxylibc_assert(x) do{if(x){}else{proxylibc_assert_debug(__FILE__,__LINE__,0,"");}}while(0)
#else
#define assert(x) ((void)0)
#endif

#define static_assert _Static_assert

