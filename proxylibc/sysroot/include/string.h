#pragma once
// string.h C99 standard header

// Legacy APIs
#define strdup proxylibc_strdup
char *strdup(const char *s);

// FIXME:
#include <stddef.h> // size_t

#define memcpy proxylibc_memcpy
void *memcpy(void * restrict s1, const void * restrict s2, size_t n);

#define memmove proxylibc_memmove
void *memmove(void *s1, const void *s2, size_t n);

#define strcpy proxylibc_strcpy
char *strcpy(char * restrict s1, const char * restrict s2);

#define strncpy proxylibc_strncpy
char *strncpy(char * restrict s1, const char * restrict s2, size_t n);

#define strcat proxylibc_strcat
char *strcat(char * restrict s1, const char * restrict s2);

#define strncat proxylibc_strncat
char *strncat(char * restrict s1, const char * restrict s2, size_t n);

#define memcmp proxylibc_memcmp
int memcmp(const void *s1, const void *s2, size_t n);

#define strcmp proxylibc_strcmp
int strcmp(const char *s1, const char *s2);

#define strcoll proxylibc_strcoll
int strcoll(const char *s1, const char *s2);

#define strncmp proxylibc_strncmp
int strncmp(const char *s1, const char *s2, size_t n);

#define strxfrm proxylibc_strxfrm
size_t strxfrm(char * restrict s1, const char * restrict s2, size_t n);

#define memchr proxylibc_memchr
void *memchr(const void *s, int c, size_t n);

#define strchr proxylibc_strchr
char *strchr(const char *s, int c);

#define strcspn proxylibc_strcspn
size_t strcspn(const char *s1, const char *s2);

#define strpbrk proxylibc_strpbrk
char *strpbrk(const char *s1, const char *s2);

#define strrchr proxylibc_strrchr
char *strrchr(const char *s, int c);

#define strspn proxylibc_strspn
size_t strspn(const char *s1, const char *s2);

#define strstr proxylibc_strstr
char *strstr(const char *s1, const char *s2);

#define strtok proxylibc_strtok
char *strtok(char * restrict s1, const char * restrict s2);

#define memset proxylibc_memset
void *memset(void *s, int c, size_t n);

#define strerror proxylibc_strerror
char *strerror(int errnum);

#define strlen proxylibc_strlen
size_t strlen(const char *s);
