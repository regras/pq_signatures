- PQCgenKAT_sign.c file was modified: for(i=0;i<100;i++) --> for(i=0;i<1;i++): Only one set of (msg, pk, sk, sm)

- PQCgenKAT_sign_check_v1.c
1) Read mlen, m, sk, pk and sm from file
2) sm is calculated using crypto_sign function
3) m1 is calculate using crypto_sign_open using sm and pk
4) m and m1 are compared

- PQCgenKAT_sign_check.c
1) mlen and m are read from msg.txt
2) sk is read from sk.txt
3) sm is printed into sig.txt
4) pk is read from pk.txt
5) m1 is printed into m1.txt

Sequence of commands:
1) make clean
2) make
3) ./PQCgenKAT_sign
4) Create file msg.txt, sk.txt and pk.txt using data from PQCgenKAT64_rsp
5) make check
6) ./check
7) The generate signature of message from msg.txt and secret key from sk.txt will be printed into sig.txt file
8) Original message m and message m1 from signature from file (sm) and message m2 from signature calculate through message and secret key got from msg.txt and sk.txt, respectively, are printed into messages.txt file
