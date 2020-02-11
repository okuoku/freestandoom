#include <stdlib.h>
#include <stdio.h>
#include <rfb/rfb.h>
#include <rfb/keysym.h>
#include "../iopkt/common/iopkt-host.h"
#include "../doomgeneric/doomgeneric/doomkeys.h"

int host_load(const char* path);
int host_init(void);
int host_step(uint32_t);
int host_keyq(uint32_t,uint32_t);

// mode X x2
#define WIDTH 640
#define HEIGHT 400

uint32_t keys = 0;

static uint32_t
transkeypos(rfbKeySym k){
    switch(k){
        case XK_Return:
            return KEY_ENTER;
        case XK_Escape:
            return KEY_ESCAPE;
        case XK_Left:
            return KEY_LEFTARROW;
        case XK_Right:
            return KEY_RIGHTARROW;
        case XK_Up:
            return KEY_UPARROW;
        case XK_Down:
            return KEY_DOWNARROW;
        case XK_space:
            return KEY_FIRE;
        case XK_w:
            return KEY_USE;
        case XK_e:
            return KEY_RSHIFT;
        default:
            return 0;
    }
}

static void
keyevent(rfbBool down, rfbKeySym k, rfbClientPtr rc){
    uint32_t x,y;
    (void)rc;
    x = down ? 1 : 0;
    y = transkeypos(k);
    printf("KEY: %d %d\n",x,y);
    if(y){
        host_keyq(x,y);
    }
}

static enum rfbNewClientAction
acceptclient(rfbClientPtr cl){
    (void)cl;
    return RFB_CLIENT_ACCEPT;
}

int
main(int ac, char** av){
    int r;
    int d;
    uint32_t addr;

    rfbScreenInfoPtr rs;
    rfbClientPtr rc;

    rs = rfbGetScreen(NULL, NULL, WIDTH, HEIGHT, 8, 4, 4);
    rs->frameBuffer = malloc(WIDTH*HEIGHT*4);
    rs->newClientHook = acceptclient;
    rs->kbdAddEvent = keyevent;
    rs->serverFormat.redShift = 16;
    rs->serverFormat.blueShift = 0;
    rc = rfbReverseConnection(rs, "127.0.0.1", 5500);

    d = 0;
    r = host_load("freestandoom.wasm");
    if(! r){
        addr = host_init();
        for(;;){
            // FIXME: It doesn't handle keys...
            rfbProcessEvents(rs, 0);
            //printf("Step: %d\n", d);
            host_step(keys);
            iopkt_copyin32(addr, rs->frameBuffer, WIDTH*HEIGHT*4);
            rfbMarkRectAsModified(rs, 0,0,WIDTH,HEIGHT);
            d++;
        }
    }
}
