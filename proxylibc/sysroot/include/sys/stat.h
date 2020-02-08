#pragma once
// FIXME: TENTATIVE

typedef int mode_t;

#define mkdir proxylibc_mkdir
int mkdir(const char *path, mode_t mode);
