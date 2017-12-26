#include "../include/fsize.h"
#include <cstdio>
long fsize(FILE* fptr){
    long ret_val = -1;
    if(fptr){
        long current_pos = ftell(fptr);
        fseek(fptr, 0L, SEEK_END);
        ret_val = ftell(fptr);
        fseek(fptr, current_pos, SEEK_SET);
    }
    return ret_val;
}
