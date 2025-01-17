#include "poly1305.h"

#include <string.h>

/**
 * Adapted from poly1305aes_test_clamp.c version 20050207
 * D. J. Bernstein
 * Public domain.
 * The function was originally named poly1305aes_test_clamp
 */
void clamp(uchar r[16]) {
    r[3] &= 15;
    r[7] &= 15;
    r[11] &= 15;
    r[15] &= 15;
    r[4] &= 252;
    r[8] &= 252;
    r[12] &= 252;
}

void poly1305(uchar key[32], uchar **M, int q, uchar out[16]) {
    // Copying key to avoid modifying input
    uchar RS[32];
    memcpy(RS, key, 32);

    mpz_t T, R_mpz, S_mpz, c_i, mod, two_128;
    mpz_inits(T, R_mpz, S_mpz, c_i, mod, two_128, NULL);

    // Initializing two_128 = 2^128
    mpz_set_ui(two_128, 2);
    mpz_pow_ui(two_128, two_128, 128);

    // Initializing mod = 2^130 - 5
    mpz_set_ui(mod, 2);
    mpz_pow_ui(mod, mod, 130);
    mpz_sub_ui(mod, mod, 5);

    // 1
    le_bytes_to_num(&RS[16], S_mpz);

    // 2
    clamp(&RS[0]);
    le_bytes_to_num(&RS[0], R_mpz);

    // 3
    mpz_set_ui(T, 0);

    // 4
    for (int i = 0; i < q; ++i) {
        // 5
        le_bytes_to_num(M[i], c_i);
        if (i < q - 1)
            mpz_add(c_i, c_i, two_128);

#if DEBUG
        gmp_printf("Block %d : %Zx\n", i, c_i);
#endif

        // 6
        mpz_add(T, T, c_i);
        mpz_mul(T, R_mpz, T);
        mpz_mod(T, T, mod);

#if DEBUG
        gmp_printf("Acc after block %d : %Zx\n", i, T);
#endif
    }

    // 7
    mpz_add(T, T, S_mpz);
    mpz_mod(T, T, mod);

    // 8
    mpz_mod(T, T, two_128);
    le_bytes_from_num(T, out);

    mpz_clears(T, R_mpz, S_mpz, c_i, mod, two_128, NULL);
}
