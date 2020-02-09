#include <stdio.h>
#include <stdlib.h>

void
proxylibc_assert_debug(const char* file, int line, int col, const char* func){
    fprintf(stderr, "%s:%d:%d: Assert (%s)\n",
            file,
            line,
            col,
            func);
    exit(EXIT_FAILURE);
}
