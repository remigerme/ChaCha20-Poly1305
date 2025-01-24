#include <stdint.h>
#include <stdio.h>

#include "chacha20.h"

// See section 2.2.1
int main() {
    uint32_t state[16] = {0x879531e0, 0xc5ecf37d, 0x516461b1, 0xc9a62f8a,
                          0x44c20ef3, 0x3390af7f, 0xd9fc690b, 0x2a5f714c,
                          0x53372767, 0xb00a5631, 0x974c541a, 0x359e9963,
                          0x5c971061, 0x3d631689, 0x2098d9d6, 0x91dbd320};

    printf("Initial state :\n");
    print_state(state);

    quarter_round(state, 2, 7, 8, 13);

    printf("After applying QUARTERROUND(2, 7, 8, 13) :\n");
    print_state(state);

    uint32_t expected_state[16] = {
        0x879531e0, 0xc5ecf37d, 0xbdb886dc, 0xc9a62f8a, 0x44c20ef3, 0x3390af7f,
        0xd9fc690b, 0xcfacafd2, 0xe46bea80, 0xb00a5631, 0x974c541a, 0x359e9963,
        0x5c971061, 0xccc07c79, 0x2098d9d6, 0x91dbd320};

    int equal = 1;
    for (int i = 0; i < 16; ++i)
        equal &= state[i] == expected_state[i];

    if (equal)
        printf("Quarter round was successfull.\n");
    else
        printf("Quarter round failed.\n");

    return 1 - equal;
}
