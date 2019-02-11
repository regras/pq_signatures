CC = /usr/bin/gcc
CFLAGS = -Wall -g -O3 -Wextra -Wpedantic
LDLIBS = -lcrypto

SOURCES = ../../params.c ../../hash.c ../../fips202.c ../../hash_address.c ../../randombytes.c ../../wots.c ../../xmss.c ../../xmss_core.c ../../xmss_commons.c ../../utils.c ../../print.c ../../read.c
HEADERS = ../../params.h ../../hash.h ../../fips202.h ../../hash_address.h ../../randombytes.h ../../wots.h ../../xmss.h ../../xmss_core.h ../../xmss_commons.h ../../utils.h ../../print.h ../../read.h

fingerprint: fingerprint.c $(SOURCES) $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $< $(LDLIBS)
