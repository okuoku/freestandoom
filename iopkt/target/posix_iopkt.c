#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <errno.h>

#include "../common/iopkt.h"

int
mkdir(const char* path, mode_t mode){
    uint32_t req[IOPKT_REQ_MAX];
    uint32_t res[IOPKT_RES_MAX];

    (void)mode;

    req[0] = IOPKT_PATH_OP_MKDIR;
    req[1] = (uintptr_t)path;

    iopkt_req32(req, res);

    if(res[0]){
        // FIXME: Map errno here.
        errno = res[0];
        return -1;
    }else{
        return 0;
    }

}
