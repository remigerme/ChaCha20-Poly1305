CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 # -g -DDEBUG
INCLUDES = -lm -lgmp -Isrc

all: poly1305-gen poly1305-check chacha20 aead_wrap aead_unwrap

poly1305-gen: src/utils.c src/poly1305.c src/poly1305_gen.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

poly1305-check: src/utils.c src/poly1305.c src/poly1305_check.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

chacha20: src/utils.c src/chacha20.c src/chacha20_bin.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

aead_wrap: src/utils.c src/chacha20.c src/poly1305.c src/aead.c src/aead_wrap.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

aead_unwrap: src/utils.c src/chacha20.c src/poly1305.c src/aead.c src/aead_unwrap.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

tests: all quarter-round
	./quarter-round
	./tests/chacha20/test.sh
	./tests/aead/test.sh

quarter-round: src/chacha20.c tests/chacha20/quarter_round.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f poly1305-gen poly1305-check chacha20 aead_wrap aead_unwrap quarter-round
