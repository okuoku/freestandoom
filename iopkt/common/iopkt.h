#pragma once

#include <stdint.h>

#define IOPKT_GRP_PATH 1
#define IOPKT_GRP_FILE 2

#define IOPKT_REQID(grp,op) ((IOPKT_GRP_ ## grp<<16) + op)

// [OPEN, zone, path, mode, new?, append?] => [res, zone, id_L, id_H]
#define IOPKT_PATH_OP_OPEN IOPKT_REQID(PATH,1) 
#define IOPKT_PATH_OP_OPEN_MODE_RW 1
#define IOPKT_PATH_OP_OPEN_MODE_RO 2
#define IOPKT_PATH_OP_OPEN_MODE_WO 3
// [REMOVE, path]
#define IOPKT_PATH_OP_REMOVE IOPKT_REQID(PATH,2) 
// [RENAME, old, new]
#define IOPKT_PATH_OP_RENAME IOPKT_REQID(PATH,3) 
// [MKDIR, path]
#define IOPKT_PATH_OP_MKDIR IOPKT_REQID(PATH,4) 

#define IOPKT_PATH_ZONE_DEBUG 1
#define IOPKT_PATH_ZONE_DEBUG_STDIN 1
#define IOPKT_PATH_ZONE_DEBUG_STDOUT 2
#define IOPKT_PATH_ZONE_DEBUG_STDERR 3
#define IOPKT_PATH_ZONE_HOSTFS 2

// [CLOSE, zone, id_L, id_H] => [res]
#define IOPKT_FILE_OP_CLOSE IOPKT_REQID(FILE,1)
// [READ, zone, id_L, id_H, off_L, off_H, buf, len] => [res, len]
#define IOPKT_FILE_OP_READ IOPKT_REQID(FILE,2)
// [WRITE, zone, id_L, id_H, off_L, off_H, buf, len] => [res, len]
#define IOPKT_FILE_OP_WRITE IOPKT_REQID(FILE,3)
// [SIZE, zone, id_L, id_H] => [res, len_L, len_H]
#define IOPKT_FILE_OP_SIZE IOPKT_REQID(FILE, 4)

#define IOPKT_REQ_MAX 16
#define IOPKT_RES_MAX 4

void iopkt_req32(uint32_t* req, uint32_t* res);
