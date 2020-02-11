#include <stdio.h>
#include "doomgeneric/doomgeneric/doomkeys.h"
#include "doomgeneric/doomgeneric/doomgeneric.h"

extern int global_buttonbitmap;
extern uint64_t my_framecount;

uint32_t keyq_pressed[32];
uint32_t keyq_keyname[32];
uint32_t keyq_rptr = 0;
uint32_t keyq_wptr = 0;

// Exports
void
RX_keyqadd(int pressed, int keyname){
    uint32_t w;

    w = keyq_wptr;
    keyq_wptr ++;
    if(keyq_wptr == 32){
        keyq_wptr = 0;
    }
    if(keyq_wptr == keyq_rptr){
        printf("KeyQ drop!\n");
        keyq_wptr = w;
    }else{
        keyq_pressed[w] = pressed;
        keyq_keyname[w] = keyname;
    }
}

void
DG_Init(){
    my_framecount = 0;
}

void
DG_DrawFrame(){
    printf("Drawframe!\n");
}

void
DG_SleepMs(uint32_t ms){
    printf("Sleep for %d\n",ms);
    (void)ms;
}

uint32_t
DG_GetTicksMs(){
    uint32_t tick;
    tick = my_framecount * 1000 / 20;
    printf("Tick: %d\n",tick);
    return tick;
}

int
DG_GetKey(int *pressed, unsigned char* key){
    if(keyq_rptr == keyq_wptr){
        return 0;
    }else{
        *pressed = keyq_pressed[keyq_rptr];
        *key = keyq_keyname[keyq_rptr];
        keyq_rptr++;
        if(keyq_rptr == 32){
            keyq_rptr = 0;
        }
        return 1;
    }
}

void
DG_SetWindowTitle(const char* title){
    printf("Window title: %s\n",title);
}

