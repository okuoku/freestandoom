void M_FindResponseFile(void);
void dg_Create(void);
void D_DoomMain();

#include <stdio.h>
int errno;
FILE* proxylibc_stderr = NULL;

// in heap.c
void heap_init(void);

//__attribute__((export_name(main))) // For newer LLVM
__attribute__((__visibility__("default")))
void
run(void) {
    heap_init();

    M_FindResponseFile();
    dg_Create();
    D_DoomMain();
}
