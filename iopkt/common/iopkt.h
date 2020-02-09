#pragma once

#include <stdint.h>

#define IOPKT_GRP_PATH 1
#define IOPKT_GRP_FILE 2

#define IOPKT_PATH_OP_OPEN 1 // [zone, path, mode, new?, append?] => [res, zone, id_L, id_H]
#define IOPKT_PATH_OP_OPEN_MODE_RW 1
#define IOPKT_PATH_OP_OPEN_MODE_RO 2
#define IOPKT_PATH_OP_OPEN_MODE_WO 3
#define IOPKT_PATH_OP_REMOVE 2 // [path]
#define IOPKT_PATH_OP_RENAME 3 // [old, new]
#define IOPKT_PATH_OP_MKDIR 4 // [path]

#define IOPKT_PATH_ZONE_DEBUG 1
#define IOPKT_PATH_ZONE_DEBUG_STDIN 1
#define IOPKT_PATH_ZONE_DEBUG_STDOUT 2
#define IOPKT_PATH_ZONE_DEBUG_STDERR 3
#define IOPKT_PATH_ZONE_HOSTFS 2

#define IOPKT_FILE_OP_CLOSE 1 // [zone, id_L, id_H] => [res]
#define IOPKT_FILE_OP_READ 2 // [zone, id_L, id_H, off_L, off_H, buf, len] => [res, len]
#define IOPKT_FILE_OP_WRITE 3 // [zone, id_L, id_H, off_L, off_H, buf, len] => [res, len]
#define IOPKT_FILE_OP_SIZE 4 // [zone, id_L, id_H] => [res, len_L, len_H]

#define IOPKT_REQ_MAX 16
#define IOPKT_RES_MAX 4

void iopkt_req32(uint32_t* req, uint32_t* res);
