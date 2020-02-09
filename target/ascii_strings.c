#include <strings.h>
#include <ctype.h>

int
strcasecmp(const char* s1, const char *s2){
    int c1,c2;
    size_t off;

    for(off = 0;;off++){
        c1 = toupper(*(s1 + off));
        c2 = toupper(*(s2 + off));
        if(!c1){
            if(!c2){
                return 0;
            }else{
                return -1;
            }
        }
        if(!c2){
            return 1;
        }
        if(c1 == c2){
            continue;
        }else if(c1 > c2){
            return 1;
        }else{
            return -1;
        }
    }
}

int
strncasecmp(const char* s1, const char *s2, size_t n){
    int c1,c2;
    size_t off;

    for(off = 0;off != n;off++){
        c1 = toupper(*(s1 + off));
        c2 = toupper(*(s2 + off));
        if(!c1){
            if(!c2){
                return 0;
            }else{
                return -1;
            }
        }
        if(!c2){
            return 1;
        }
        if(c1 == c2){
            continue;
        }else if(c1 > c2){
            return 1;
        }else{
            return -1;
        }
    }

    return 0;
}
