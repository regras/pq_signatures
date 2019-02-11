#include <stdio.h>

#include "../../params.h"
#include "../../read.h"
#include "../../utils.h"
#include "../../hash.h"

/**
 * Helper method for pseudorandom key generation.
 * Expands an n-byte array into a len*n byte array using the `prf` function.
 */

int main(){
	
	FILE *fp;
	
	uint32_t i;
    unsigned char ctr[32];	
	
	int j;
	
	xmss_params params;
    uint32_t oid = 0x00000001;
	xmss_parse_oid(&params, oid);
	
	xmss_params *params1;
	
	fp = fopen("values.txt","r");
	
	unsigned char seed[params.n];
	unsigned char pk[params.wots_sig_bytes];
	
	params1 = &params;
	
	
	// seed read from a file
	if ( !ReadHex(fp, seed, 32, "seed = ") ) {
         printf("ERROR: unable to read 'seed' from values.txt\n");
    }
	
	for(j=0; j<32; j++)
		printf("%02X\n",seed[j]);
	
	
	for (i = 0; i < params.wots_len; i++) {
        ull_to_bytes(ctr, 32, i);
		prf(params1, &pk[i*params.n], ctr, seed);
		for(j=0; j<(int)params.n; j++)
			printf("%02X",pk[j+i*params.n]);
		printf("\n");
	}
	
	
	
	
	fclose(fp);
	
}