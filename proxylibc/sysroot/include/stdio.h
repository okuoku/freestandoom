#pragma once
// stdio.h C99 standard header


// FIXME:
#include <stdarg.h> // va_list
#include <stddef.h> // NULL, size_t
#include <stdint.h> // uint64_t

struct proxylibc_FILE_s;
typedef struct proxylibc_FILE_s FILE;

typedef uint64_t fpos_t;

// _IOFBF
// _IOLBF
// _IONBF
// BUFSIZ
#define EOF proxylibc_EOF
#define proxylibc_EOF -1

// FOPEN_MAX
// FILENAME_MAX
// L_tmpnam
#define SEEK_CUR proxylibc_SEEK_CUR
#define proxylibc_SEEK_CUR 1
#define SEEK_END proxylibc_SEEK_END
#define proxylibc_SEEK_END 2
#define SEEK_SET proxylibc_SEEK_SET
#define proxylibc_SEEK_SET 0
// TMP_MAX

#define stderr proxylibc_stderr
extern FILE* stderr;
#define stdin proxylibc_stdin
extern FILE* stdin;
#define stdout proxylibc_stdout
extern FILE* stdout;

#define remove proxylibc_remove
int remove(const char *filename);

#define rename proxylibc_rename
int rename(const char *old, const char *new);

// tmpfile
// tmpnam

#define fclose proxylibc_fclose
int fclose(FILE *stream);

#define fflush proxylibc_fflush
int fflush(FILE *stream);

#define fopen proxylibc_fopen
FILE* fopen(const char * restrict filename, const char * restrict mode);

// freopen

// setbuf

// setvbuf

#define fprintf proxylibc_fprintf
int fprintf(FILE * restrict stream, const char * restrict format, ...);

// fscanf

#define printf proxylibc_printf
int printf(const char * restrict format, ...);

// scanf

#define snprintf proxylibc_snprintf
int snprintf(char * restrict s, size_t n, const char * restrict format, ...);

// sprintf

#define sscanf proxylibc_sscanf
int sscanf(const char * restrict s, const char * restrict format, ...);

// vfprintf
#define vfprintf proxylibc_vfprintf
int vfprintf(FILE * restrict stream, const char * restrict format, va_list arg);
// vfscanf
// vprintf
// vscanf
#define vsnprintf proxylibc_vsnprintf
int vsnprintf(char * restrict s, size_t n, const char * restrict format, va_list arg);
// vsprintf
#define vsscanf proxylibc_vsscanf
int vsscanf(const char * restrict s, const char * restrict format, va_list arg);

// fgetc
// fgets
// fputc
// fputs
// getc
// getchar
// putc
#define putchar proxylibc_putchar
int putchar(int c);
#define puts proxylibc_puts
int puts(const char *s);

// ungetc

#define fread proxylibc_fread
size_t fread(void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream);

#define fwrite proxylibc_fwrite
size_t fwrite(const void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream);

#define fgetpos proxylibc_fgetpos
int fgetpos(FILE * restrict stream, fpos_t * restrict pos);

#define fseek proxylibc_fseek
int fseek(FILE *stream, long int offset, int whence);

#define fsetpos proxylibc_fsetpos
int fsetpos(FILE *stream, const fpos_t *pos);

#define ftell proxylibc_ftell
long int ftell(FILE *stream);

#define rewind proxylibc_rewind
void refind(FILE *stream);

// clearerr
// feof
// ferror
// perror

