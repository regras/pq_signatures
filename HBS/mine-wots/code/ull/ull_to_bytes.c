#include <stdio.h>
#include "print.h"

void ull_to_bytes(unsigned char *out, unsigned int outlen, unsigned long long in);
unsigned long long bytes_to_ull(const unsigned char *in, unsigned int inlen);

int main(){
	
	FILE *fp;
	
	unsigned long long num, num1;
	
	//uint32_t i;
    unsigned char ctr[32];
	
	fp = fopen("ull_bytes.txt","w");
	
	printf("Digite um número: \n");
	scanf("%llu",&num);
	
	ull_to_bytes(ctr,32,num);
	
	fprintBstr(fp, "ull = ", ctr, 32);
	
	fclose(fp);
	
	num1 = bytes_to_ull(ctr,32);
	
	printf("O numero digitado foi: %llu\n",num1);
	
	return 1;
	
}

/**
 * Converts the value of 'in' to 'outlen' bytes in big-endian byte order.
 */
void ull_to_bytes(unsigned char *out, unsigned int outlen,
                  unsigned long long in)
{
    int i;

    /* Iterate over out in decreasing order, for big-endianness. */
    for (i = outlen - 1; i >= 0; i--) {
        out[i] = in & 0xff;
        in = in >> 8;
    }
}

/**
 * Converts the inlen bytes in 'in' from big-endian byte order to an integer.
 */
unsigned long long bytes_to_ull(const unsigned char *in, unsigned int inlen)
{
    unsigned long long retval = 0;
    unsigned int i;

    for (i = 0; i < inlen; i++) {
        retval |= ((unsigned long long)in[i]) << (8*(inlen - 1 - i));
    }
    return retval;
}