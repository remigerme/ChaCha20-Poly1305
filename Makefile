CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 # -g -DDEBUG
INCLUDES = -lm -lgmp -Isrc

all: poly1305-gen poly1305-check

poly1305-gen: src/utils.c src/poly1305.c src/poly1305_gen.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

poly1305-check: src/utils.c src/poly1305.c src/poly1305_check.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

tests: quarter-round
	./quarter-round

quarter-round: src/chacha20.c tests/quarter_round.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f poly1305-gen poly1305-check quarter_round
