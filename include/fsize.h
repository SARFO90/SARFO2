#ifndef FSIZE_H
#define FSIZE_H
#include <cstdio>
/**
 * \param [in] fptr  the file pointer
 * \return           a) (-1), if fptr == NULL
 *                   b) the size of the file in bytes, otherwise
 */
long fsize(FILE* fptr);
#endif
