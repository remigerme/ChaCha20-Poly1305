#include "aead.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 6) {
        fprintf(stderr,
                "[Usage] ./aead_wrap key-file nonce ad-file plain-file "
                "cipher-file\nWhere :\n\tkey-file : name of a "
                "64-byte binary file\n\tnonce : a 24-character hexadecimal "
                "string representing a 12-byte nonce\n\tad-file : name of the "
                "binary associated data file \n\tplain-file : name "
                "of the binary plaintext input file\n\tcipher-file : name of "
                "the (new) binary output file containing ciphertext\nThe "
                "computed tag will be printed on standard output\n");
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

    // Retrieving plain
    FILE *input_fd = fopen(argv[4], "rb");
    int plain_size = get_file_size(input_fd);
    char *plain = (char *)malloc(sizeof(char) * plain_size);
    fread(plain, sizeof(char), plain_size, input_fd);
    fclose(input_fd);

    // Computing ciphertext and tag
    uchar *ciphertext = (uchar *)malloc(sizeof(uchar) * plain_size);
    uchar tag[16];
    aead(Wrap, key, nonce, aad, aad_size, plain, plain_size, ciphertext, tag);

    // Writing ciphertext data to file
    FILE *output_fd = fopen(argv[5], "wb");
    fwrite(ciphertext, sizeof(char), plain_size, output_fd);
    fclose(output_fd);

    // Printing tag (hex-encoded)
    for (int i = 0; i < 16; ++i)
        printf("%02x", tag[i]);
    printf("\n");

    // Free stuff
    free(ciphertext);
    free(aad);
    free(plain);
}
