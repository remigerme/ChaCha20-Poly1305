# ChaCha20 and Poly1305

The code have been implemented using [RFC 8439](https://datatracker.ietf.org/doc/html/rfc8439).

## Installation

```shell
make
```

This builds five targets : `poly1305-gen`, `poly1305-check`, `chacha20`, `aead_wrap` and `aead_unwrap`.

## Tests

### ChaCha20 & AEAD

The following command will compile and run tests automatically :

```shell
make tests
```

Information about performed tests will be displayed. If a test fails it returns an error code, otherwise it returns with code 0 if successfull.

### Poly1305

For now, there are only some manual tests available.

```shell
./poly1305-gen $(cat tests/key.txt) tests/msg.txt
```

```shell
./poly1305-check $(cat tests/key.txt) tests/msg.txt a8061dc1305136c6c22b8baf0c0127a9
```

```shell
./poly1305-check $(cat tests/key.txt) tests/msg.txt a8061dc1305136c6c22b8baf0c0127a8
```

Code implemented in the context for "Advanced Cryptology" course at École polytechnique by Rémi Germe.
