#ifndef POLY1305_H
#define POLY1305_H

#include "utils.h"

/**
 * TODO
 */
void poly1305(uchar key[32], uchar **M, int q, uchar out[16]);

#endif // POLY1305_H
