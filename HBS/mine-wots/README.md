## XMSS reference code [![Build Status](https://travis-ci.org/joostrijneveld/xmss-reference.svg?branch=master)](https://travis-ci.org/joostrijneveld/xmss-reference)

This repository contains the reference implementation that accompanies the Internet Draft _"XMSS: Extended Hash-Based Signatures"_, [`draft-irtf-cfrg-xmss-hash-based-signatures`](https://datatracker.ietf.org/doc/draft-irtf-cfrg-xmss-hash-based-signatures/).

This reference implementation supports all parameter sets as defined in the Draft at run-time (specified by prefixing the public and private keys with a 32-bit `oid`). Implementations that want to use compile-time parameter sets can remove the `struct xmss_params` function parameter.

_When using the current code base, please be careful, expect changes and watch this document for further documentation. In particular, `xmss_core_fast.c` is long due for a serious clean-up. While this will not change its public API or output, it may affect the storage format of the BDS state (i.e. part of the secret key)._

### Dependencies

For the SHA-2 hash functions (i.e. SHA-256 and SHA-512), we rely on OpenSSL. Make sure to install the OpenSSL development headers. On Debian-based systems, this is achieved by installing the OpenSSL development package `libssl-dev`.

### License

This reference implementation was written by Andreas Hülsing and Joost Rijneveld. All included code is available under the CC0 1.0 Universal Public Domain Dedication.
