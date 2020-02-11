#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../tlsf/tlsf.h"

static tlsf_t heap_ctx;
static uint8_t *heap;
#define HEAP_PAGES (1024)
#define PAGE_SIZE (64*1024)
#define HEAPSIZE (HEAP_PAGES*PAGE_SIZE)

void
heap_init(void){
    int r;
    /* Find last page */
    heap = (uint8_t*)((uintptr_t)__builtin_wasm_memory_size(0) * 64 * 1024);
    /* grow memory */
    r = __builtin_wasm_memory_grow(0, HEAP_PAGES);
    if(r<0){
        for(;;);
    }
    heap_ctx = tlsf_create_with_pool(heap, HEAPSIZE);
}

void*
malloc(size_t size){
    return tlsf_malloc(heap_ctx, size);
}

void
free(void* ptr){
    tlsf_free(heap_ctx, ptr);
}

void*
realloc(void* ptr, size_t size){
    return tlsf_realloc(heap_ctx, ptr, size);
}

void*
calloc(size_t nmemb, size_t size){
    void *p;
    const size_t total = nmemb * size;
    p = malloc(total);
    memset(p, 0, total);
    return p;
}
