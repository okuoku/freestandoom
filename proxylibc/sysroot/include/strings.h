#pragma once
// strings.h : POSIX https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/strings.h.html

// FIXME:
#include <stddef.h> // size_t

// ffs

#define strcasecmp proxylibc_strcasecmp
int strcasecmp(const char *, const char *);

// strcasecmp_l
#define strncasecmp proxylibc_strncasecmp
int strncasecmp(const char*, const char*, size_t);

// strncasecmp_l
