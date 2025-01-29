#include "chacha20.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @param x Int to be rotated
 * @param i Number of bits to rotate (0 <= i < 32)
 * @return x rotated by i bits to the left
 */
uint32_t rotl_32(uint32_t x, size_t i) {
    assert(i < 32);
    if (i == 0)
        return x;
    return (x << i) | (x >> (32 - i));
}

void print_state(uint32_t state[16]) {
    for (int i = 0; i < 16; ++i) {
        printf("%08x ", state[i]);
        if ((i + 1) % 4 == 0)
            printf("\n");
    }
}

void quarter_round(uint32_t state[16], int a, int b, int c, int d) {
    state[a] += state[b];
    state[d] ^= state[a];
    state[d] = rotl_32(state[d], 16);
    state[c] += state[d];
    state[b] ^= state[c];
    state[b] = rotl_32(state[b], 12);
    state[a] += state[b];
    state[d] ^= state[a];
    state[d] = rotl_32(state[d], 8);
    state[c] += state[d];
    state[b] ^= state[c];
    state[b] = rotl_32(state[b], 7);
}

void chacha20_block(uint32_t key[8], uint32_t nonce[3], uint32_t count,
                    uchar out[64]) {
    // Initializing state
    uint32_t state[16] = {0x61707865, 0x3320646e, 0x79622d32, 0x6b206574,
                          key[0],     key[1],     key[2],     key[3],
                          key[4],     key[5],     key[6],     key[7],
                          count,      nonce[0],   nonce[1],   nonce[2]};
    uint32_t initial_state[16];
    memcpy(initial_state, state, 16 * sizeof(uint32_t));

    // Rounds
    for (int i = 1; i <= 10; ++i) {
        // Inner state
        quarter_round(state, 0, 4, 8, 12);
        quarter_round(state, 1, 5, 9, 13);
        quarter_round(state, 2, 6, 10, 14);
        quarter_round(state, 3, 7, 11, 15);
        quarter_round(state, 0, 5, 10, 15);
        quarter_round(state, 1, 6, 11, 12);
        quarter_round(state, 2, 7, 8, 13);
        quarter_round(state, 3, 4, 9, 14);
    }

    // Final addition
    for (int i = 0; i < 16; ++i)
        state[i] += initial_state[i];

    // Serializing
    memcpy(out, state, 64);
}

void chacha20_encrypt(uint32_t key[8], uint32_t initial_counter,
                      uint32_t nonce[3], char *plain, int plain_size,
                      uchar *encrypted) {
    uchar key_stream[64];

    // Treating full blocks
    for (int j = 0; j < plain_size / 64; ++j) {
        chacha20_block(key, nonce, initial_counter + j, key_stream);
        for (int i = 0; i < 64; ++i)
            encrypted[64 * j + i] = plain[64 * j + i] ^ key_stream[i];
    }

    // Last block might be smaller
    if (plain_size % 64 != 0) {
        int j = plain_size / 64;
        chacha20_block(key, nonce, initial_counter + j, key_stream);
        for (int i = 0; i < plain_size % 64; ++i)
            encrypted[64 * j + i] = plain[64 * j + i] ^ key_stream[i];
    }
}
