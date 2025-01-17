# poly1305

The code have been implemented using [RFC 8439](https://datatracker.ietf.org/doc/html/rfc8439), especially using section 2.5.

## Installation

```shell
make
```

This builds two targets : `poly1305-gen` and `poly1305-check`.

## Tests

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
