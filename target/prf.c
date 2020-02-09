#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int
snprintf(char *s, size_t n, const char* format, ...){
    int r;
    va_list ap;

    va_start(ap, format);
    r = vsnprintf(s, n, format, ap);
    va_end(ap);
    return r;
}

int
printf(const char *format, ...){
    int r;
    va_list ap;

    va_start(ap, format);
    r = vfprintf(stdout, format, ap);
    va_end(ap);
    return r;
}

int
fprintf(FILE* stream, const char *format, ...){
    int r;
    va_list ap;

    va_start(ap, format);
    r = vfprintf(stream, format, ap);
    va_end(ap);
    return r;
}

int
puts(const char* s){
    size_t len,r;
    len = strlen(s);
    r = fwrite(s, 1, len, stdout);
    if(r != len){
        return EOF;
    }else{
        return 1;
    }
}

int
putchar(int c){
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    return puts(buf);
}

int
sscanf(const char* s, const char* format, ...){
    int r;
    va_list ap;

    va_start(ap, format);
    r = vsscanf(s, format, ap);
    va_end(ap);
    return r;
}
