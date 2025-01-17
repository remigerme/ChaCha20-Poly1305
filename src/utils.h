#ifndef UTILS_H
#define UTILS_H

#include <gmp.h>

typedef unsigned char uchar;

/**
 * TODO
 */
void hextouchars(char *in, int n, uchar *out);

/**
 * TODO
 */
void le_bytes_to_num(uchar rs[16], mpz_t RS);

/**
 * TODO
 */
void le_bytes_from_num(mpz_t T, uchar out[16]);

#endif // UTILS_H
