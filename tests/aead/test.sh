#!/bin/bash

DIR="tests/chacha20"

KEYFILE="$DIR/keyfile"
TEMP_AAD="$DIR/aad"
TEMP_INPUT="$DIR/temp_input"
TEMP_CIPHER="$DIR/temp_cipher"
TEMP_TAG="$DIR/temp_tag"
TEMP_DECRYPTED="$DIR/temp_decrypted"

# Test : generate random nonce and binary input file
# and try to wrap then unwrap - should retrieve same file

for i in $(seq 1 10)
do
    openssl rand -out $TEMP_AAD $(shuf -i 60-80 -n 1)
    openssl rand -base64 -out $TEMP_INPUT $(shuf -i 1000-10000 -n 1)

    NONCE=$(openssl rand -hex 12)

    # sed command below is used to remove an extra newline at the end of decrypted
    TAG=$(./aead_wrap $KEYFILE $NONCE $TEMP_AAD $TEMP_INPUT $TEMP_CIPHER)
    ./aead_unwrap $KEYFILE $NONCE $TEMP_AAD $TEMP_CIPHER $TAG | sed -z 's/\n$//' > $TEMP_DECRYPTED
    
    if cmp -s $TEMP_INPUT $TEMP_DECRYPTED; then
        echo "Successfully wrapped then unwrapped random inputs with random nonce"
    else
        echo "Something went wrong when wrapping / unwrapping random inputs"
        exit 1
    fi
done

rm -f $TEMP_CIPHER $TEMP_INPUT $TEMP_DECRYPTED $TEMP_AAD
