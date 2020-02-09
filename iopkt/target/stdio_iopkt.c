#include <stdlib.h>
#include <stdio.h>

#include "../common/iopkt.h"

struct proxylibc_FILE_s {
    uint32_t zone;
    uint32_t handle_l;
    uint32_t handle_h;

    int64_t pos;
    int64_t limit;
};

FILE* proxylibc_stderr = NULL;
FILE* proxylibc_stdout = NULL;

void
stdio_init(void){
    proxylibc_stderr = malloc(sizeof(FILE));
    proxylibc_stdout = malloc(sizeof(FILE));

    proxylibc_stderr->zone = IOPKT_PATH_ZONE_DEBUG;
    proxylibc_stderr->handle_l = IOPKT_PATH_ZONE_DEBUG_STDERR;
    proxylibc_stderr->handle_h = 0;

    proxylibc_stdout->zone = IOPKT_PATH_ZONE_DEBUG;
    proxylibc_stdout->handle_l = IOPKT_PATH_ZONE_DEBUG_STDOUT;
    proxylibc_stdout->handle_h = 0;
}

/* PATH ops */
FILE*
fopen(const char* filename, const char* mode){
    FILE* fp = NULL;
    uint32_t req[IOPKT_REQ_MAX];
    uint32_t res[IOPKT_RES_MAX];

    uint32_t opmode = IOPKT_PATH_OP_OPEN_MODE_RO;
    uint32_t append = 0;
    uint32_t create = 0;

    /* parse mode parameter */
    for(;*mode;mode++){
        switch(*mode){
            case 'r':
                opmode = IOPKT_PATH_OP_OPEN_MODE_RO;
                break;
            case '+':
                create = 1;
                break;
            case 'w':
                opmode = IOPKT_PATH_OP_OPEN_MODE_RW;
                break;
            case 'a':
                opmode = IOPKT_PATH_OP_OPEN_MODE_RW;
                append = 1;
                break;
            case 'b':
                /* Ignore */
                break;
            default:
                fprintf(stderr, "fopen: Unknown modeline %s\n", mode);
                break;
        }
    }


    req[0] = IOPKT_PATH_OP_OPEN;
    req[1] = IOPKT_PATH_ZONE_HOSTFS;
    req[2] = (uintptr_t)filename;
    req[3] = opmode;
    req[4] = create;
    req[5] = append;

    iopkt_req32(req, res);

    if(!res[0]){
        fp = malloc(sizeof(FILE));
        if(fp){
            fp->zone = res[1];
            fp->handle_l = res[2];
            fp->handle_h = res[3];
            
            /* Calc current file size */
            req[0] = IOPKT_FILE_OP_SIZE;
            req[1] = fp->zone;
            req[2] = fp->handle_l;
            req[3] = fp->handle_h;
            iopkt_req32(req, res);

            if(!res[0]){
                fp->limit = res[2];
                fp->limit = (fp->limit << 32ULL);
                fp->limit += res[1];
            }else{
                fp->limit = -1;
            }

            /* If append, seek to end */
            if(append){
                fp->pos = fp->limit;
            }else{
                fp->pos = 0;
            }
        }
    }else{
        fprintf(stderr, "fopen: Error %s , %d\n", filename, res[1]);
        return NULL;
    }

    return fp;
}

int
remove(const char* filename){
    uint32_t req[IOPKT_REQ_MAX];
    uint32_t res[IOPKT_RES_MAX];

    req[0] = IOPKT_PATH_OP_REMOVE;
    req[1] = (uintptr_t)filename;

    iopkt_req32(req, res);

    return res[0];
}

int
rename(const char* oldname, const char* newname){
    uint32_t req[IOPKT_REQ_MAX];
    uint32_t res[IOPKT_RES_MAX];

    req[0] = IOPKT_PATH_OP_RENAME;
    req[1] = (uintptr_t)oldname;
    req[2] = (uintptr_t)newname;

    iopkt_req32(req, res);

    return res[0];
}

/* FILE ops */
int
fclose(FILE* stream){
    uint32_t req[IOPKT_REQ_MAX];
    uint32_t res[IOPKT_RES_MAX];

    if(stream->zone == IOPKT_PATH_ZONE_DEBUG){
        fprintf(stderr, "fclose: Ignore closing standard I/O\n");
        return 0;
    }

    req[0] = IOPKT_FILE_OP_CLOSE;
    req[1] = stream->zone;
    req[2] = stream->handle_l;
    req[3] = stream->handle_h;

    iopkt_req32(req, res);

    if(res[0]){
        fprintf(stderr, "fclose: Ignored error %d\n", res[0]);
    }

    return 0;
}

size_t
fread(void* restrict ptr, size_t size, size_t nmemb, FILE* restrict stream){
    uint32_t req[IOPKT_REQ_MAX];
    uint32_t res[IOPKT_RES_MAX];
    uint64_t totalsize;
    totalsize = size;
    totalsize *= nmemb;

    req[0] = IOPKT_FILE_OP_READ;
    req[1] = stream->zone;
    req[2] = stream->handle_l;
    req[3] = stream->handle_h;
    req[4] = (uint32_t)stream->pos;
    req[5] = (stream->pos >> 32ULL);
    req[6] = (uintptr_t)ptr;
    req[7] = totalsize;

    iopkt_req32(req, res);

    if(res[0]){
        return 0;
    }
    if(stream->limit >= 0){
        stream->pos += res[1];
        if(stream->limit < stream->pos){
            stream->limit = stream->pos;
        }
    }
    return res[1] / size;
}

size_t
fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream){
    uint32_t req[IOPKT_REQ_MAX];
    uint32_t res[IOPKT_RES_MAX];
    uint64_t totalsize;
    totalsize = size;
    totalsize *= nmemb;

    req[0] = IOPKT_FILE_OP_WRITE;
    req[1] = stream->zone;
    req[2] = stream->handle_l;
    req[3] = stream->handle_h;
    req[4] = (uint32_t)stream->pos;
    req[5] = (stream->pos >> 32ULL);
    req[6] = (uintptr_t)ptr;
    req[7] = totalsize;

    iopkt_req32(req, res);

    if(res[0]){
        return 0;
    }
    if(stream->limit >= 0){
        stream->pos += res[1];
        if(stream->limit < stream->pos){
            stream->limit = stream->pos;
        }
    }
    return res[1] / size;
}

int
fflush(FILE* stream){
    /* Do nothing */
    return 0;
}

long int
ftell(FILE* stream){
    return stream->pos;
}

int
fseek(FILE* stream, long int offset, int whence){
    uint32_t req[IOPKT_REQ_MAX];
    uint32_t res[IOPKT_RES_MAX];
    /* First, update size */
    req[0] = IOPKT_FILE_OP_SIZE;
    req[1] = stream->zone;
    req[2] = stream->handle_l;
    req[3] = stream->handle_h;

    iopkt_req32(req, res);

    if(!res[0]){
        stream->limit = res[2];
        stream->limit = (stream->limit << 32ULL);
        stream->limit += res[1];
    }else{
        stream->limit = -1;
    }

    switch(whence){
        case SEEK_CUR:
            stream->pos += offset;
            break;
        case SEEK_END:
            stream->pos = stream->limit - offset;
            break;
        case SEEK_SET:
            stream->pos = offset;
            break;
        default:
            fprintf(stderr, "fseek: Ignored unknown whence parameter %d\n", whence);
            break;
    }

    if(stream->pos > stream->limit){
        stream->pos = stream->limit;
    }
    if(stream->pos < 0){
        stream->pos = 0;
    }
    return 0;
}
