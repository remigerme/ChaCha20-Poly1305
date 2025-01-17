#include "poly1305.h"

#include <assert.h>
#include <gmp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr,
                "[Usage] ./poly1305-gen key filename\n\tWhere key is a 32-byte "
                "sequence given as a 64-character hexadecimal string\n");
        return 1;
    }

    // Retrieving key
    uchar key[32];
    hextouchars(argv[1], 32, key);

#if DEBUG
    printf("Retrieved key :\n");
    for (int i = 0; i < 32; ++i)
        printf("%02hhx", key[i]);
    printf("\n");
#endif

    // Opening file
    FILE *fd = fopen(argv[2], "rb");
    // Retrieving file size
    // https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(fd, 0, SEEK_END);
    int fsize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    // Number of chunks (including padding)
    int q = ceil((double)(fsize + 1) / 16.0);
    uchar **M = (uchar **)malloc(sizeof(uchar *) * q);
    uchar *buf = (uchar *)malloc(sizeof(uchar) * fsize);
    fread(buf, sizeof(char), fsize, fd); // read only in one time
    for (int i = 0; i < fsize; ++i) {
        int chunk = i / 16;
        int byte = i % 16;
        if (byte == 0)
            M[chunk] = (uchar *)calloc(16, sizeof(char));
        M[chunk][byte] = buf[i];
    }
    fclose(fd);

    // Padding, add 1 and 0s (done with calloc)
    M[fsize / 16][fsize % 16] = 0x01;

#if DEBUG
    printf("Read file :\n");
    for (int i = 0; i < q; ++i)
        for (int j = 0; j < 16; ++j)
            printf("%02hhx", M[i][j]);
    printf("\n");
#endif

    uchar out[16];
    poly1305(key, M, q, out);

    for (int i = 0; i < 16; ++i)
        printf("%02hhx", out[i]);
    printf("\n");

    for (int i = 0; i < q; ++i)
        free(M[i]);
    free(M);
}
