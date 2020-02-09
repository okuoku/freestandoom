// "dummy" sscanf implementation, for freestandoom

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

static void*
next_pointer(va_list arg){
    return va_arg(arg, void*);
}

static const char*
consume_ws(const char* s){
    for(;isspace(*s);s++){
    }
    return s;
}

static int 
dignum(int c){
    switch(c){
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'a':
        case 'A':
            return 10;
        case 'b':
        case 'B':
            return 11;
        case 'c':
        case 'C':
            return 12;
        case 'd':
        case 'D':
            return 13;
        case 'e':
        case 'E':
            return 14;
        case 'f':
        case 'F':
            return 15;
        default:
            return -1;
    }
}

static const char*
consume_hex(const char* s, va_list arg, int* out_count){
    int valid = 0;
    int64_t acc = 0;
    int d;
    int *out;
    for(;;){
        d = dignum(*s);
        if(d < 0){
            break;
        }
        valid = 1;
        acc *= 16;
        acc += d;
        s++;
    }
    if(valid){
        out = next_pointer(arg);
        *out = acc;
        *out_count = (*out_count) + 1;
    }
    return s;
}

static const char*
consume_oct(const char* s, va_list arg, int* out_count){
    int valid = 0;
    int64_t acc = 0;
    int d;
    int *out;
    for(;;){
        d = dignum(*s);
        if(d < 0 || d > 8){
            break;
        }
        valid = 1;
        acc *= 8;
        acc += d;
        s++;
    }
    if(valid){
        out = next_pointer(arg);
        *out = acc;
        *out_count = (*out_count) + 1;
    }
    return s;
}

static const char*
consume_dec(const char* s, va_list arg, int* out_count){
    int valid = 0;
    int64_t acc = 0;
    int d;
    int *out;
    for(;;){
        d = dignum(*s);
        if(d < 0 || d > 10){
            break;
        }
        valid = 1;
        acc *= 10;
        acc += d;
        s++;
    }
    if(valid){
        out = next_pointer(arg);
        *out = acc;
        *out_count = (*out_count) + 1;
    }
    return s;
}

static const char*
consume_int(const char* s, va_list arg, int* out_count){
    if(s[0] == 0){
        return s;
    }
    if(s[1] == 0){
        return consume_dec(s, arg, out_count);
    }
    if(s[0] == '0' && s[1] == 'x'){
        return consume_hex(s + 2, arg, out_count);
    }
    if(s[0] == '0' && s[1] == 'X'){ // ???
        return consume_hex(s + 2, arg, out_count);
    }
    if(s[0] == '0'){
        return consume_oct(s + 1, arg, out_count);
    }
    return consume_dec(s, arg, out_count);
}

int
vsscanf(const char* s, const char* format, va_list arg){
    int current_command = 0;
    int consumed_pointers = 0;
    
    for(;*format;format++){
        if(*s == 0){
            break;
        }
        if(isspace(*format)){
            /* Consume all whitespace */
            s = consume_ws(s);
        }else if(current_command){
            switch(*format){
                case 'x':
                    current_command = 0;
                    s = consume_hex(s, arg, &consumed_pointers);
                    break;
                case 'd':
                    current_command = 0;
                    s = consume_dec(s, arg, &consumed_pointers);
                    break;
                case 'o':
                    current_command = 0;
                    s = consume_oct(s, arg, &consumed_pointers);
                    break;
                case 'i':
                    current_command = 0;
                    s = consume_int(s, arg, &consumed_pointers);
                    break;
                default:
                    fprintf(stderr, "dummy_scanf: Unknown fmt [%s]\n", format);
                    break;
            }
        }else if(*format == '%'){
            current_command = -1;
        }else if(*format == *s){
            s++;
            format++;
        }else{
            /* Unmatch */
            break;
        }
    }

    return consumed_pointers;
}
