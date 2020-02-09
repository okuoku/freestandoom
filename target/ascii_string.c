#include <string.h>

char*
strchr(const char* s, int c){
    const char* e = s + strlen(s) + 1;
    for(;s != e; s++){
        if(*s == c){
            return (char *)s; /* Deconst */
        }
    }
    return 0;
}

char*
strrchr(const char* s, int c){
    const char* r = s + strlen(s) + 1;
    for(;r != s; r--){
        if(*r == c){
            return (char *)r; /* Deconst */
        }
    }
    return 0;
}

char*
strncpy(char* s1, const char* s2, size_t n){
    int c;
    size_t off;
    for(off = 0; off != n; off++){
        c = *(s2 + off);
        *(s1 + off) = c;
        if(!c){
            break;
        }
    }
    return s1;
}

size_t
strlen(const char* s){
    size_t r = 0;
    while(*s){
        r++;
        s++;
    }
    return r;
}

int
strcmp(const char* s1, const char* s2){
    int c1,c2;
    size_t off;

    for(off = 0;;off++){
        c1 = *(s1 + off);
        c2 = *(s2 + off);
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
strncmp(const char* s1, const char* s2, size_t n){
    int c1,c2;
    size_t off;

    for(off = 0; off != n;off++){
        c1 = *(s1 + off);
        c2 = *(s2 + off);
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


