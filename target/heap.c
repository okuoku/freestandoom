#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../tlsf/tlsf.h"

#define HEAPSIZE (1024*1024*64)

static tlsf_t heap_ctx;
static uint8_t heap[HEAPSIZE];

void
heap_init(void){
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
