#include "../include/get_init_state.h"
int get_init_state(char32_t sym, const State_for_char sts[], int n){
    int lower, upper, middle;
    lower = 0; upper = n - 1;
    while(lower <= upper){
        middle = (lower + upper) >> 1;
        char32_t c_ = sts[middle].c;
        if(sym == c_){
            return sts[middle].st;
        }else if(sym > c_){
            lower = middle + 1;
        }else{
            upper = middle - 1;
        }
    }
    return -1;
}
