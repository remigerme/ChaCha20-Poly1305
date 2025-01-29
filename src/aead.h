#ifndef AEAD_H
#define AEAD_H

#include "utils.h"

#include <stdint.h>

/**
 * See section 2.6
 */
void poly1305_key_gen(uint32_t key[8], uint32_t nonce[3], uchar out[32]);

/**
 * See section 2.8
 */
void aead(uint32_t key[8], uint32_t nonce[3], char *aad, int aad_size,
          char *plain, int plain_size, uchar *ciphertext, uchar tag[16]);

#endif // AEAD_H
