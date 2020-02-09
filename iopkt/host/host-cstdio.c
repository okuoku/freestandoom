#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../common/iopkt.h"
#include "../common/iopkt-host.h"

#define MAX_FILES 128
static FILE* files[MAX_FILES];

static FILE*
files_lookup(uint32_t lo, uint32_t hi){
    if(hi == 0 && (lo < MAX_FILES)){
        return files[lo];
    }else{
        return NULL;
    }
}

static void
files_add(FILE* fp, uint32_t* out_lo, uint32_t* out_hi){
    int i;
    for(i=0;i!=MAX_FILES;i++){
        if(!files[i]){
            files[i] = fp;
            *out_lo = i;
            *out_hi = 0;
            return;
        }
    }
    *out_lo = -1;
    return;
}

static void
files_del(FILE* fp, uint32_t lo, uint32_t hi){
    if(hi == 0 && (lo < MAX_FILES)){
        if(files[lo] != fp){
            fprintf(stderr, "files_del: TRYING TO REMOVE WRONG FILE!!\n");
        }
        files[lo] = NULL;
    }
}

static char*
copyindup(uint32_t loc){
    void* p;
    int r;
    p = malloc(1024);
    r = iopkt_copyinstr32(loc, p, 1024, NULL);
    if(r){
        free(p);
        return NULL;
    }
    return p;
}

static void
path_op_open(uint32_t zone, uint32_t path_loc, uint32_t mode, 
             uint32_t do_create, uint32_t do_append,
             uint32_t* out_res,
             uint32_t* out_zone, uint32_t* out_id_l, uint32_t* out_id_h){
    char* path;
    char* req;
    FILE* fp;
    path = copyindup(path_loc);

    if(mode == IOPKT_PATH_OP_OPEN_MODE_RO){
        req = "rb";
    }else{
        if(do_append){
            if(do_create){
                req = "a+b";
            }else{
                req = "ab";
            }
        }else{
            if(do_create){
                req = "w+b";
            }else{
                req = "wb";
            }
        }
    }

    fp = fopen(path, req);

    if(fp){
        *out_res = 0;
        *out_zone = IOPKT_PATH_ZONE_HOSTFS;
        files_add(fp, out_id_l, out_id_h);
    }else{
        *out_res = 1;
    }

    if(path){
        free(path);
    }
}

static void
path_op_remove(uint32_t path_loc, uint32_t* out_res){
    char* path;
    path = copyindup(path_loc);

    *out_res = remove(path);

    if(path){
        free(path);
    }
}

static void
path_op_rename(uint32_t oldpath_loc, uint32_t newpath_loc, uint32_t* out_res){
    char* oldpath;
    char* newpath;
    oldpath = copyindup(oldpath_loc);
    newpath = copyindup(newpath_loc);

    *out_res = rename(oldpath, newpath);

    if(oldpath){
        free(path);
    }
    if(newpath){
        free(path);
    }
}

static void
path_op_mkdir(uint32_t path_loc, uint32_t* out_res){
    fprintf(stderr, "mkdir: Ignored %s\n", NULL);
    *out_res = 0;
}

static void
file_op_close(uint32_t zone, uint32_t id_l, uint32_t id_h, uint32_t* out_res){
    FILE* fp;
    fp = files_lookup(id_l,id_h);

    if(!fp){
        *out_res = -1;
    }else{
        fclose(fp);
        files_del(fp,id_l,id_h);
        *out_res = 0;
    }
}

static void
file_op_read(uint32_t zone, uint32_t id_l, uint32_t id_h,
             uint32_t off_l, uint32_t off_h,
             uint32_t buf_loc, uint32_t len,
             uint32_t* out_res, uint32_t* out_len){
    int do_seek = 0;
    long int offset;
    size_t rlen;
    FILE* fp;
    void* buf;
    if(zone == IOPKT_PATH_ZONE_DEBUG){
        switch(id_l){
            case IOPKT_PATH_ZONE_DEBUG_STDIN:
                fp = stdin;
                break;
            default:
                fprintf("read: Invalid ch id\n");
                *out_res = -1;
                return;
        }
    }else{
        fp = files_lookup(id_l, id_h);
        if(!fp){
            fprintf("read: Invalid fileid %d,%d\n",id_l,id_h);
            *out_res = -1;
            return;
        }
        do_seek = 1;
    }

    if(do_seek){
        offset = (off_h << 32ULL) + off_l;
        fseek(fp, offset, SEEK_SET);
    }

    buf = malloc(len);

    rlen = fread(buf, 1, len, fp);

    iopkt_copyout32(buf, buf_loc, rlen);

    *out_res = 0;
    *out_len = rlen;

    if(buf){
        free(buf);
    }
}

static void
file_op_write(uint32_t zone, uint32_t id_l, uint32_t id_h,
              uint32_t off_l, uint32_t off_h,
              uint32_t buf_loc, uint32_t len,
              uint32_t* out_res, uint32_t* out_len){
    int do_seek = 0;
    long int offset;
    size_t wlen;
    FILE* fp;
    void* buf;
    if(zone == IOPKT_PATH_ZONE_DEBUG){
        switch(id_l){
            case IOPKT_PATH_ZONE_DEBUG_STDOUT:
                fp = stdout;
                break;
            case IOPKT_PATH_ZONE_DEBUG_STDERR:
                fp = stderr;
                break;
            default:
                fprintf("read: Invalid ch id\n");
                *out_res = -1;
                return;
        }
    }else{
        fp = files_lookup(id_l, id_h);
        if(!fp){
            fprintf("read: Invalid fileid %d,%d\n",id_l,id_h);
            *out_res = -1;
            return;
        }
        do_seek = 1;
    }

    if(do_seek){
        offset = (off_h << 32ULL) + off_l;
        fseek(fp, offset, SEEK_SET);
    }

    buf = malloc(len);
    iopkt_copyin32(buf_loc, buf, len);

    wlen = fwrite(buf, 1, len, fp);

    *out_res = 0;
    *out_len = rlen;

    if(buf){
        free(buf);
    }
}

static void
file_op_size(uint32_t zone, uint32_t id_l, uint32_t id_h,
             uint32_t* out_res, uint32_t* out_len_l, uint32_t* out_len_h){
    long int cur;
    long int r;
    FILE* fp;
    if(zone == IOPKT_PATH_ZONE_DEBUG){
        *out_res = -1;
        return;
    }
    fp = files_lookup(id_l, id_h);
    if(!fp){
        *out_res = -1;
        return;
    }
    cur = ftell(fp);
    fseek(fp, 0, SEEK_END);
    r = ftell(fp);
    fseek(fp, cur, SEEK_SET);
    *out_len_l = r;
    *out_len_h = r >> 32;
    *out_res = 0;
}

void 
iopkt_req32(uint32_t* req, uint32_t* res){
    if(!req){
        return;
    }
    switch(req[0]){
        case IOPKT_PATH_OP_OPEN:
            path_op_open(req[1],req[2],req[3],req[4],req[5],
                         &res[0], &res[1], &res[2], &res[3]);
            break;
        case IOPKT_PATH_OP_REMOVE:
            path_op_remove(req[1], &res[0]);
            break;
        case IOPKT_PATH_OP_RENAME:
            path_op_rename(req[1], req[2], &res[0]);
            break;
        case IOPKT_PATH_OP_MKDIR:
            path_op_mkdir(req[1], &res[0]);
            break;
        case IOPKT_FILE_OP_CLOSE:
            file_op_close(req[1], req[2], req[3], &res[0]);
            break;
        case IOPKT_FILE_OP_READ:
            file_op_read(req[1], req[2], req[3], 
                         req[4], req[5], req[6], req[7],
                         &res[0], &res[1]);
            break;
        case IOPKT_FILE_OP_WRITE:
            file_op_write(req[1], req[2], req[3],
                          req[4], req[5], req[6], req[7],
                          &res[0], &res[1]);
            break;
        case IOPKT_FILE_OP_SIZE:
            file_op_size(req[1], req[2], req[3],
                         &res[0], &res[1], &res[2]);
            break;
        default:
            fprintf(stderr, "iopkt: Unknown OP 0x%x\n", req[0]);
            break;
    }
}
