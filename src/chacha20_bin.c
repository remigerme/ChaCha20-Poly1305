#include "chacha20.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

int main(int argc, char **argv) {
    if (argc < 5) {
        fprintf(stderr,
                "[Usage] ./chacha20 key-file nonce input-file "
                "output-file\nWhere :\n\tkey-file : name of a "
                "64-byte binary file\n\tnonce : a 24-character hexadecimal "
                "string representing a 12-byte nonce\n\tinput-file : name "
                "of the binary input file\n\toutput-file : name of the (new) "
                "binary output file\n");
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

    // Retrieving input
    FILE *input_fd = fopen(argv[3], "rb");
    int plain_size = get_file_size(input_fd);
    char *plain = (char *)malloc(sizeof(char) * plain_size);
    fread(plain, sizeof(char), plain_size, input_fd);
    fclose(input_fd);

    // Computing encrypted data
    char *encrypted = (char *)malloc(sizeof(char) * plain_size);
    chacha20_encrypt(key, 1, nonce, plain, plain_size, encrypted);

    // Writing encrypted data to output
    FILE *output_fd = fopen(argv[4], "wb");
    fwrite(encrypted, sizeof(char), plain_size, output_fd);
    fclose(output_fd);

    // Free stuff
    free(encrypted);
    free(plain);
}
