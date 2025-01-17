#ifndef POLY1305_H
#define POLY1305_H

#include "utils.h"

/**
 * @param key The key represented as 32 bytes
 * @param M The chunks of 16 bytes of the message, last chunk is already padded
 * @param q Number of chunks of M
 * @param out Return tag which is modified
 */
void poly1305(uchar key[32], uchar **M, int q, uchar out[16]);

#endif // POLY1305_H
