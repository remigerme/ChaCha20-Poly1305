#!/bin/bash

DIR="tests/chacha20"
NONCE="000000000000004a00000000"

KEYFILE="$DIR/keyfile"
INPUT_FILE="$DIR/sunscreen.txt"
EXPECTED_CIPHER="$DIR/ciphertext.bin"
TEMP_INPUT="$DIR/temp_input"
TEMP_CIPHER="$DIR/temp_cipher"
TEMP_DECRYPTED="$DIR/temp_decrypted"

# First test : trying to encrypt using given example from RFC
./chacha20 $KEYFILE $NONCE $INPUT_FILE $TEMP_CIPHER

if cmp -s $TEMP_CIPHER $EXPECTED_CIPHER; then
    echo "Successfully encrypted sunscreen.txt"
else
    echo "Something went wrong when encrypting sunscreen.txt"
    exit 1
fi

# Second test : generate random nonce and binary input file
# and try to encrypt then decrypt - should retrieve same file

for i in $(seq 1 10)
do
    openssl rand -out $TEMP_INPUT $(shuf -i 1000-10000 -n 1)

    NONCE=$(openssl rand -hex 12)

    ./chacha20 $KEYFILE $NONCE $TEMP_INPUT $TEMP_CIPHER
    ./chacha20 $KEYFILE $NONCE $TEMP_CIPHER $TEMP_DECRYPTED

    if cmp -s $TEMP_INPUT $TEMP_DECRYPTED; then
        echo "Successfully encrypted then decrypted random input with random nonce"
    else
        echo "Something went wrong when encrypting / encrypting random input"
        exit 1
    fi
done

rm -f $TEMP_CIPHER $TEMP_INPUT $TEMP_DECRYPTED
