#include "aead.h"

#include <assert.h>
#include <string.h>

#include "chacha20.h"
#include "poly1305.h"

int pad16_size(int size) {
    if (size % 16 == 0)
        return 0;
    return 16 - size % 16;
}

int pad16(int size, uchar *buf) {
    int psize = pad16_size(size);
    if (psize == 0)
        return 0;
    memset(buf, 0, psize);
    return 1;
}

void poly1305_key_gen(uint32_t key[8], uint32_t nonce[3], uchar out[32]) {
    uchar buf[64];
    chacha20_block(key, nonce, 0, buf);
    memcpy(out, buf, 32);
}

void aead(uint32_t key[8], uint32_t nonce[3], char *aad, int aad_size,
          char *plain, int plain_size, uchar *ciphertext, uchar tag[16]) {
    // Creating otk
    uchar otk[32];
    poly1305_key_gen(key, nonce, otk);

    // Creating ciphertext
    chacha20_encrypt(key, 1, nonce, plain, plain_size, ciphertext);

    // Preparing mac_data
    // q is the number of 16 bytes chunks
    int q = aad_size + pad16_size(aad_size);  // AAD
    q += plain_size + pad16_size(plain_size); // Ciphertext
    q += 16;                                  // Respective lengths
    q /= 16;                                  // Number of chunks
    uchar **mac_data = (uchar **)malloc(sizeof(uchar *) * q);
    mac_data[0] = (uchar *)malloc(sizeof(uchar) * 16);
    int c = 0;
    int i = 0;

    // AAD
    while (16 * c + i < aad_size) {
        mac_data[c][i] = aad[16 * c + i];
        i = (i + 1) % 16;
        if (i == 0) {
            ++c;
            mac_data[c] = (uchar *)malloc(sizeof(uchar) * 16);
        }
    }
    int res = pad16(aad_size, &mac_data[c][i]);
    if (res) {
        i = 0;
        ++c;
        mac_data[c] = (uchar *)malloc(sizeof(uchar) * 16);
    }

    // Ciphertext
    int ca = c;
    while (16 * (c - ca) + i < plain_size) {
        mac_data[c][i] = ciphertext[16 * (c - ca) + i];
        i = (i + 1) % 16;
        if (i == 0) {
            ++c;
            mac_data[c] = (uchar *)malloc(sizeof(uchar) * 16);
        }
    }
    res = pad16(plain_size, &mac_data[c][i]);
    if (res) {
        ++c;
        mac_data[c] = (uchar *)malloc(sizeof(uchar) * 16);
    }

    assert(c == q - 1);

    // Respective lengths
    le_bytes_from_int(aad_size, mac_data[q - 1]);
    le_bytes_from_int(plain_size, &mac_data[q - 1][8]);

    // Creating tag
    poly1305(otk, mac_data, q, tag);
}
