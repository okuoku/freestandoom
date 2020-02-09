#include <ctype.h>
#include <stdlib.h>

int
atoi(const char* nptr){
    int inv = 1; /* invert output */
    int acc = 0;

    /* Skip whitespace */
    while(isspace(*nptr)){
        nptr++;
    }

    /* Sign */
    if(*nptr == '-'){
        inv = -1;
        nptr++;
    }else if(*nptr == '+'){
        nptr++;
    }

    while(isdigit(*nptr)){
        acc *= 10;
        acc += (*nptr - '0');
        nptr++;
    }

    /* FIXME: Overflow..? */
    return acc * inv;
}
