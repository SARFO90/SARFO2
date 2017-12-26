#include "../include/error_count.h"
#include <cstdio>

void Error_count::increment_number_of_errors(){
    number_of_errors++;
}

int Error_count::get_number_of_errors(){
    return number_of_errors;
}

void Error_count::print(){
    printf("\nTotal errors %d\n", number_of_errors);
}
