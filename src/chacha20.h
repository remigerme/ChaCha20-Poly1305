#ifndef CHACHA20_H
#define CHACHA20_H

#include "utils.h"

#include <stdint.h>
#include <stdlib.h>

void print_state(uint32_t state[16]);

/**
 * See section 2.1
 * @param state State of which to modify components a, b, c, d
 * @param a First index of state to modify
 * @param b Second index of state to modify
 * @param c Third index of state to modify
 * @param d Fourth index of state to modify
 */
void quarter_round(uint32_t state[16], int a, int b, int c, int d);

/**
 * See section 2.3
 */
void chacha20_block(uint32_t key[8], uint32_t nonce[3], uint32_t count,
                    uchar out[64]);

/**
 * See section 2.4
 */
void chacha20_encrypt(uint32_t key[8], uint32_t initial_counter,
                      uint32_t nonce[3], char *plain, int plain_size,
                      uchar *encrypted);

#endif // CHACHA20_H
