#include <stdio.h>

#include "read.h"


int main(){
	
	FILE *fp;
	
	int j;
	
	fp = fopen("values.txt","r");
	
	unsigned char seed[32];
	
	
	// seed read from a file
	if ( !ReadHex(fp, seed, 32, "seed = ") ) {
         printf("ERROR: unable to read 'seed' from values.txt\n");
    }
	
	for(j=0; j<32; j++)
		printf("%02X\n",seed[j]);
	
	fclose(fp);
	
}