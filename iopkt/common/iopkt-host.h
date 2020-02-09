#pragma once
#include <stdint.h>

int iopkt_copyin32(uint32_t addr, void* dest, uint32_t len);
int iopkt_copyout32(void* src, uint32_t addr, uint32_t len);
int iopkt_copyinstr32(uint32_t addr, void* dest, size_t maxlen, size_t *actlen);

