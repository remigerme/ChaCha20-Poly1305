#include "aead.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 6) {
        fprintf(stderr,
                "[Usage] ./aead_wrap key-file nonce ad-file cipher-file "
                "tag\nWhere :\n\tkey-file : name of a 64-byte binary "
                "file\n\tnonce : a 24-character hexadecimal string "
                "representing a 12-byte nonce\n\tad-file : name of the "
                "binary associated data file\n\tcipher-file : name of "
                "the input cipher file to try to decrypt and "
                "authenticate\n\ttag : a 32-character hexadecimal "
                "string representing a 16-byte tag\nIf the ciphertext is "
                "successfully unwraped, the plaintext is printed on standard "
                "output\n");
        return 1;
    }

    // Retrieving key
    uint32_t key[8];
    FILE *key_fd = fopen(argv[1], "rb");
    int key_size = get_file_size(key_fd);
    assert(key_size == 32);
    fread(key, sizeof(uint32_t), 8, key_fd);
    fclose(key_fd);

    // Retrieving nonce
    uint32_t nonce[3];
    hextouchars(argv[2], 12, (uchar *)nonce);

    // Retrieving aad
    FILE *aad_fd = fopen(argv[3], "rb");
    int aad_size = get_file_size(aad_fd);
    char *aad = (char *)malloc(sizeof(char) * aad_size);
    fread(aad, sizeof(char), aad_size, aad_fd);
    fclose(aad_fd);

    // Retrieving cipher
    FILE *cipher_fd = fopen(argv[4], "rb");
    int cipher_size = get_file_size(cipher_fd);
    char *cipher = (char *)malloc(sizeof(char) * cipher_size);
    fread(cipher, sizeof(char), cipher_size, cipher_fd);
    fclose(cipher_fd);

    // Retrieving tag
    uchar i_tag[16];
    hextouchars(argv[5], 16, i_tag);

    // Computing plain and tag
    uchar *plain = (uchar *)malloc(sizeof(uchar) * cipher_size);
    uchar tag[16];
    aead(Unwrap, key, nonce, aad, aad_size, cipher, cipher_size, plain, tag);

    // Check if given tag is equal to computed tag
    int tag_valid = 1;
    for (int i = 0; i < 16; ++i)
        tag_valid &= tag[i] == i_tag[i];

    // Printing the plaintext if tag is valid
    if (tag_valid)
        printf("%.*s\n", cipher_size, plain);

    // Free stuff
    free(cipher);
    free(aad);
    free(plain);
}
