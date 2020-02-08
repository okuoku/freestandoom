void M_FindResponseFile(void);
void dg_Create(void);
void D_DoomMain();

#include <stdio.h>
int errno;
FILE* proxylibc_stderr = NULL;


//__attribute__((export_name(main))) // For newer LLVM
__attribute__((__visibility__("default")))
void
run(void) {
    M_FindResponseFile();
    dg_Create();
    D_DoomMain();
}
