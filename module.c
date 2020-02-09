void M_FindResponseFile(void);
void dg_Create(void);
void D_DoomMain();

#include <stdio.h>
int errno;

// in stdio_iopkt.c
void stdio_init(void);

// in heap.c
void heap_init(void);

// Globals
int global_buttonbitmap;
static int state;

//__attribute__((export_name(init))) // For newer LLVM
__attribute__((__visibility__("default")))
void
init(void) {
    heap_init();
    stdio_init();

    state = 0;

    M_FindResponseFile();
    dg_Create();
    D_DoomMain();
}


int RX_DoomLoopStep(int state);

//__attribute__((export_name(step))) // For newer LLVM
__attribute__((__visibility__("default")))
void
step(int buttons){
    global_buttonbitmap = buttons;
    state = RX_DoomLoopStep(state);
}
