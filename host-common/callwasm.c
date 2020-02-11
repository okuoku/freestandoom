#include <stdio.h> // host_load
#include <string.h>
#include <stdlib.h>
#include "../iopkt/common/iopkt.h"
#include "../iopkt/common/iopkt-host.h"
#include "wasm.h"

/* Global states */
wasm_engine_t* engine;
wasm_store_t* store;
wasm_memory_t* export_memory; /* 0 */
wasm_func_t* export_init; /* 1 */
wasm_func_t* export_step; /* 2 */
wasm_func_t* export_keyq; /* 3 */

/* WASM I/O functions */
int
iopkt_copyin32(uint32_t addr, void* dest, uint32_t len){
    void* x;
    if(!addr){
        fprintf(stderr, "Trying to read from addr 0\n");
    }
    // FIXME: Check boundary
    x = wasm_memory_data(export_memory);
    memcpy(dest, x + addr, len);
    return 0;
}
int
iopkt_copyout32(void* src, uint32_t addr, uint32_t len){
    void* x;
    if(!addr){
        fprintf(stderr, "Trying to write to addr 0\n");
    }
    // FIXME: Check boundary
    x = wasm_memory_data(export_memory);
    memcpy(x + addr, src, len);
    return 0;
}

int
iopkt_copyinstr32(uint32_t addr, void* dest, size_t maxlen, size_t* actlen){
    char* x;
    char* y;
    int c;
    size_t cnt;
    if(!addr){
        fprintf(stderr, "Trying to read from addr 0\n");
    }
    x = wasm_memory_data(export_memory);
    y = dest;
    for(cnt = 0; cnt != maxlen; cnt++){
        c = x[cnt + addr];
        //printf("%lx+%lx: [%c]\n",addr,cnt,c);
        if(c == 0){
            y[cnt] = 0;
            break;
        }else{
            y[cnt] = c;
        }
    }
    if(actlen){
        *actlen = cnt - 1;
    }
    return 0;
}

/* WASM intrinsics(memcpy, memset, memmove) */
static wasm_trap_t*
cb_memcpy(const wasm_val_t* args, wasm_val_t* results){ // 3_1
    uint32_t arg0;
    uint32_t arg1;
    uint32_t arg2;
    char* x;

    x = wasm_memory_data(export_memory);
    arg0 = args[0].of.i32;
    arg1 = args[1].of.i32;
    arg2 = args[2].of.i32;

    memcpy(&x[arg0], &x[arg1], arg2);

    results[0].kind = WASM_I32;
    results[0].of.i32 = arg0;
    return NULL;
}

static wasm_trap_t*
cb_memset(const wasm_val_t* args, wasm_val_t* results){ // 3_1
    uint32_t arg0;
    uint32_t arg1;
    uint32_t arg2;
    char* x;

    x = wasm_memory_data(export_memory);
    arg0 = args[0].of.i32;
    arg1 = args[1].of.i32;
    arg2 = args[2].of.i32;

    memset(&x[arg0], arg1, arg2);

    results[0].kind = WASM_I32;
    results[0].of.i32 = arg0;
    return NULL;
}

static wasm_trap_t*
cb_memmove(const wasm_val_t* args, wasm_val_t* results){ // 3_1
    uint32_t arg0;
    uint32_t arg1;
    uint32_t arg2;
    char* x;

    x = wasm_memory_data(export_memory);
    arg0 = args[0].of.i32;
    arg1 = args[1].of.i32;
    arg2 = args[2].of.i32;

    memmove(&x[arg0], &x[arg1], arg2);

    results[0].kind = WASM_I32;
    results[0].of.i32 = arg0;
    return NULL;
}

/* WASM callbacks(iopkt_req32, exit) */
static wasm_trap_t*
cb_exit(const wasm_val_t* args, wasm_val_t* results){ // 1_0
    uint32_t arg0;
    arg0 = args[0].of.i32;
    fprintf(stderr, "Called exit(%d)\n", arg0);
    exit(arg0);
    return NULL;
}

static wasm_trap_t* 
cb_iopkt_req32(const wasm_val_t* args, wasm_val_t* results){ // 2_0
    uint32_t arg0;
    uint32_t arg1;
    uint32_t req[IOPKT_REQ_MAX];
    uint32_t res[IOPKT_RES_MAX];
    int i;
    void* x;

    arg0 = args[0].of.i32;
    arg1 = args[1].of.i32;
    x = wasm_memory_data(export_memory);
    // FIXME: use copyin/out
    memcpy(req, x + arg0, sizeof(uint32_t) * IOPKT_REQ_MAX);
#if 0
    fprintf(stderr, "iopkt: => ");
    for(i=0;i!=IOPKT_REQ_MAX;i++){
        fprintf(stderr, "0x%x ", req[i]);
    }
#endif
    iopkt_req32(req, res);
#if 0
    fprintf(stderr, "=> ");
    for(i=0;i!=IOPKT_RES_MAX;i++){
        fprintf(stderr, "0x%x ", res[i]);
    }
    fprintf(stderr, "\n");
#endif
    memcpy(x + arg1, res, sizeof(uint32_t) * IOPKT_RES_MAX);

    return NULL;
}

/* exports */
uint32_t
host_init(void){
    wasm_val_t res[] = { /* res */ {.kind = WASM_I32, .of = {.i32 = 0}} };
    wasm_func_call(export_init, NULL, res);
    return res[0].of.i32;
}
void
host_step(uint32_t buttons){
    wasm_val_t args[] = { /* arg0 */ {.kind = WASM_I32, .of = {.i32 = 0}} };
    args[0].of.i32 = buttons;
    wasm_func_call(export_step, args, NULL);
}
void
host_keyq(uint32_t pressed, uint32_t keyname){
    wasm_val_t args[] = { 
        /* arg0 */ {.kind = WASM_I32, .of = {.i32 = 0}} ,
        /* arg1 */ {.kind = WASM_I32, .of = {.i32 = 0}} 
    };
    args[0].of.i32 = pressed;
    args[1].of.i32 = keyname;
    wasm_func_call(export_keyq, args, NULL);
}
int /* non-zero for error */
host_load(const char* path){
    size_t size;
    FILE* fp;
    wasm_byte_vec_t binary;
    wasm_module_t* module;
    wasm_instance_t* instance;
    wasm_functype_t* functype_1_0;
    wasm_functype_t* functype_2_0;
    wasm_functype_t* functype_3_1;
    wasm_func_t* func_cb_memcpy; // 3_1
    wasm_func_t* func_cb_memset; // 3_1
    wasm_func_t* func_cb_memmove; // 3_1
    wasm_func_t* func_cb_exit; // 1_0
    wasm_func_t* func_cb_iopkt_req32; // 2_0
    wasm_extern_vec_t exports;

    // Init runtime
    engine = wasm_engine_new();
    store = wasm_store_new(engine);

    // Read module
    fp = fopen(path, "rb");
    if(!fp){
        fprintf(stderr, "file not found: %s\n",path);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    wasm_byte_vec_new_uninitialized(&binary, size);
    fread(binary.data, size, 1, fp);
    fclose(fp);

    module = wasm_module_new(store, &binary);
    if(!module){
        fprintf(stderr, "failed to compile module.");
        return -1;
    }
    wasm_byte_vec_delete(&binary);

    // Set callbacks
    functype_1_0 = wasm_functype_new_1_0(wasm_valtype_new_i32());
    functype_2_0 = wasm_functype_new_2_0(wasm_valtype_new_i32(),
                                         wasm_valtype_new_i32());
    functype_3_1 = wasm_functype_new_3_1(wasm_valtype_new_i32(),
                                         wasm_valtype_new_i32(),
                                         wasm_valtype_new_i32(),
                                         wasm_valtype_new_i32());

    func_cb_memcpy = wasm_func_new(store, functype_3_1, cb_memcpy);
    func_cb_memset = wasm_func_new(store, functype_3_1, cb_memset);
    func_cb_memmove = wasm_func_new(store, functype_3_1, cb_memmove);

    func_cb_exit = wasm_func_new(store, functype_1_0, cb_exit);
    func_cb_iopkt_req32 = wasm_func_new(store, functype_2_0, cb_iopkt_req32);

    // FIXME: wabt does not implement `wasm_module_imports` so we need to 
    //        hardcode indices
    const wasm_extern_t* imports[] = 
    { 
        /* proxylibc_memcpy */ 
        wasm_func_as_extern(func_cb_memcpy),
        /* proxylibc_memset */
        wasm_func_as_extern(func_cb_memset),
        /* proxylibc_exit */
        wasm_func_as_extern(func_cb_exit),
        /* iopkt_req32 */
        wasm_func_as_extern(func_cb_iopkt_req32),
        /* proxylibc_memmove */
        wasm_func_as_extern(func_cb_memmove),
        /* memcpy */
        wasm_func_as_extern(func_cb_memcpy)
    };
    instance = wasm_instance_new(store, module, imports , NULL);

    if(!instance){
        fprintf(stderr, "Couldn't instantiate module");
        return -1;
    }

    wasm_func_delete(func_cb_memcpy);
    wasm_func_delete(func_cb_memset);
    wasm_func_delete(func_cb_memmove);

    wasm_func_delete(func_cb_exit);
    wasm_func_delete(func_cb_iopkt_req32);


    // Handle module exports
    // FIXME: Implement this with reflection
    wasm_instance_exports(instance, &exports);

    export_memory = wasm_extern_as_memory(exports.data[0]);
    export_init = wasm_extern_as_func(exports.data[1]);
    export_step = wasm_extern_as_func(exports.data[2]);
    export_keyq = wasm_extern_as_func(exports.data[3]);

    // FIXME: OK?
    //wasm_extern_vec_delete(&exports);

    wasm_module_delete(module);
    wasm_instance_delete(instance);
    return 0;
}
