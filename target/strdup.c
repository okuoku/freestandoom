#include <stdlib.h>
#include <string.h>

char *
strdup(const char* s){
    char* r;
    size_t slen;
    slen = strlen(s) + 1;

    r = malloc(slen);

    memcpy(r,s,slen);

    return r;
}
