#include <stdint.h>
#include <stdio.h>

void M_FindResponseFile(void);
void dg_Create(void);
void D_DoomMain();
extern uint32_t* DG_ScreenBuffer;

int errno;
uint64_t my_framecount;

// in stdio_iopkt.c
void stdio_init(void);

// in heap.c
void heap_init(void);

// Globals
int global_buttonbitmap;
static int state;

//__attribute__((export_name(init))) // For newer LLVM
__attribute__((__visibility__("default")))
uintptr_t
init(void) {
    heap_init();
    stdio_init();

    state = 0;

    M_FindResponseFile();
    dg_Create();
    D_DoomMain();

    printf("DoomMain returned\n");
    my_framecount = 1;

    printf("Screen Buffer = %x\n", (uintptr_t)DG_ScreenBuffer);

    return (uintptr_t)DG_ScreenBuffer;
}


int RX_DoomLoopStep(int state);
void RX_keyqadd(int pressed, int keyname);

//__attribute__((export_name(step))) // For newer LLVM
__attribute__((__visibility__("default")))
void
step(int buttons){
    global_buttonbitmap = buttons;
    state = RX_DoomLoopStep(state);
    my_framecount++;
}


//__attribute__((export_name(keyq))) // For newer LLVM
__attribute__((__visibility__("default")))
void
keyq(int a, int b){
    RX_keyqadd(a, b);
}


