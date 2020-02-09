#include <stdio.h>
#define STB_SPRINTF_IMPLEMENTATION
#include "../stb/stb_sprintf.h"

static char*
cb_outstream(char const* buf, void* u, int len){
    FILE* fp = (FILE *)u;
    fwrite(buf, len, 1, fp);
    return (char *)buf; /* Deconst */
}

int
vfprintf(FILE* stream, const char* format, va_list arg){
    char buf[STB_SPRINTF_MIN];
    return stbsp_vsprintfcb(cb_outstream, stream, buf, format, arg);
}

int
vsnprintf(char* s, size_t n, const char* format, va_list arg){
    return stbsp_vsnprintf(s, n, format, arg);
}
