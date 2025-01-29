#include "utils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void hextouchars(char *in, int n, uchar *out) {
    assert((int)strlen(in) == 2 * n);
    char buf[3];
    buf[2] = '\0';
    char *end;
    for (int i = 0; i < n; ++i) {
        buf[0] = in[2 * i];
        buf[1] = in[2 * i + 1];
        out[i] = strtoul(buf, &end, 16);
        assert(buf + 2 == end);
    }
}

void le_bytes_to_num(uchar rs[16], mpz_t RS) {
    mpz_set_ui(RS, 0);

    for (int i = 15; i >= 0; --i) {
        mpz_mul_2exp(RS, RS, 8);
        mpz_add_ui(RS, RS, rs[i]);
    }
}

void le_bytes_from_num(mpz_t T, uchar out[16]) {
    // Copy to avoid modifying input
    mpz_t temp;
    mpz_init_set(temp, T);

    for (int i = 0; i < 16; i++) {
        out[i] = (uchar)(mpz_get_ui(temp) & 0xFF);
        mpz_fdiv_q_2exp(temp, temp, 8);
    }

    mpz_clear(temp);
}

void le_bytes_from_int(uint64_t x, uchar out[8]) {
    for (int i = 0; i < 8; i++) {
        out[i] = (uchar)(x & 0xFF);
        x >>= 8;
    }
}

int get_file_size(FILE *fd) {
    fseek(fd, 0, SEEK_END);
    int size = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    return size;
}
