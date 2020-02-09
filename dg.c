#include "doomgeneric/doomgeneric/doomkeys.h"
#include "doomgeneric/doomgeneric/doomgeneric.h"

extern int global_buttonbitmap;
static uint64_t framecount = 0;

void
DG_Init(){
    framecount = 0;
}

void
DG_DrawFrame(){
}

void
DG_SleepMs(uint32_t ms){
    (void)ms;
}

uint32_t
DG_GetTicksMs(){
    return framecount * 60 * 1000;
}

int
DG_GetKey(int *pressed, unsigned char* key){
    return 0;
}

void
DG_SetWindowTitle(const char* title){
}

