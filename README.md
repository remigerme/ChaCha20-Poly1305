# ChaCha20 and Poly1305

The code have been implemented using [RFC 8439](https://datatracker.ietf.org/doc/html/rfc8439)

## Installation

```shell
make
```

This builds three targets : `poly1305-gen`, `poly1305-check` and `chacha20`.

## Tests

### ChaCha20

The following command will compile and run tests automatically :

```shell
make tests
```

Information will be displayed. If a test fails it returns an error code, otherwise it returns with code 0 if successfull.

You can also manually test the `chacha20` program using the files provided in `tests/chacha20`. Note that this example used the following nonce : `000000000000004a00000000`.

You can also generate nonce for your own tests :

```shell
openssl rand -hex 12
```

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
