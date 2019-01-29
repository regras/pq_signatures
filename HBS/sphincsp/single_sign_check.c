
/*
//  PQCgenKAT_sign_check.c
//
// Created by Antônio Unias on 1/27/19.
// This file obtains message (msg), secret key (sk) and signature (sig) from file PQCsingKAT_64.rsp.
// Then, a SPHINCS+ signature is generated using crypto_sign() function and this same signature is 
// verified using crypto_sign_open() function.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rng.h"
#include "api.h"

#define	MAX_MARKER_LEN		50

#define KAT_SUCCESS          0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_DATA_ERROR      -3
#define KAT_CRYPTO_FAILURE  -4

int	FindMarker(FILE *infile, const char *marker);
int	ReadHex(FILE *infile, unsigned char *A, int Length, char *str);
void	fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L);

char	AlgName[] = "My Alg Name";

int
main()
{
    char                		fn_rsp[32];
	FILE                		*fp_sk, *fp_pk, *fp_sig, *fp_m1;
    unsigned char       	*sm, *m1;
    unsigned long long  smlen, mlen1;
    unsigned char       	pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES];
    int                 		ret_val;
	int 						mlen = 33;
    
    printf("Entrando no programa...\n");
	
	///////////////////////////////////// secret key  ////////////////////////////////////////////////////////
	
	if ( (fp_sk = fopen("sk.txt", "r")) == NULL ) {
        printf("Couldn't open sk.txt for read\n");
        return KAT_FILE_OPEN_ERROR;
    }
	
	// Secret key
	if ( !ReadHex(fp_sk, sk, CRYPTO_SECRETKEYBYTES, "sk = ") ) {
         printf("ERROR: unable to read 'sk' from sk.txt\n");
         return KAT_DATA_ERROR;
    }
	else
		printf("sk ok!\n");
	
	fclose(fp_sk);
	
	printf("Secret key: \n");
	for(int i = 0; i<CRYPTO_SECRETKEYBYTES;i++)
		printf("%02X",sk[i]);
	printf("\n");
	
	///////////////////////////////////// secret key  ////////////////////////////////////////////////////////
	
	///////////////////////////////////// public key  ////////////////////////////////////////////////////////
	
	if ( (fp_pk = fopen("pk.txt", "r")) == NULL ) {
        printf("Couldn't open pk.txt for read\n");
        return KAT_FILE_OPEN_ERROR;
    }
	
	// Public key
	if ( !ReadHex(fp_pk, pk, CRYPTO_PUBLICKEYBYTES, "pk = ") ) {
         printf("ERROR: unable to read 'pk' from pk.txt\n");
         return KAT_DATA_ERROR;
    }
	
	fclose(fp_pk);
	
	printf("Public key: \n");
	for(int i = 0; i<CRYPTO_PUBLICKEYBYTES;i++)
		printf("%02X",pk[i]);
	printf("\n");
	
	///////////////////////////////////// public key  ////////////////////////////////////////////////////////
	
	///////////////////////////////////// signature ////////////////////////////////////////////////////////
	
	// Signature calculation
	sprintf(fn_rsp, "PQCsignKAT_%d.rsp", CRYPTO_SECRETKEYBYTES);
	if ( (fp_sig = fopen(fn_rsp, "r")) == NULL ) {
        printf("Couldn't open sig.txt for read\n");
        return KAT_FILE_OPEN_ERROR;
    }
	
	//sm = (unsigned char *)calloc(mlen+CRYPTO_BYTES, sizeof(unsigned char));
	sm = (unsigned char *)calloc(8113, sizeof(unsigned char));
	printf("sig\n");
	// Signature
	if ( !ReadHex(fp_sig, sm, mlen+CRYPTO_BYTES, "sm = ") ) {
         printf("ERROR: unable to read 'sm' from sig.txt\n");
         return KAT_DATA_ERROR;
    }
	
	fclose(fp_sig);
	
	///////////////////////////////////// signature ////////////////////////////////////////////////////////
        
    
	///////////////////////////////////// signature verification form public key  ////////////////////////////////////////////////////////
	
	// Signature verification form public key
	m1 = (unsigned char *)calloc(mlen+CRYPTO_BYTES, sizeof(unsigned char));
	if ( (ret_val = crypto_sign_open(m1, &mlen1, sm, smlen, pk)) != 0) {
        printf("crypto_sign_open returned <%d>\n", ret_val);
        return KAT_CRYPTO_FAILURE;
    }
        
    /*if ( mlen != mlen1 ) {
        printf("crypto_sign_open returned bad 'mlen': Got <%llu>, expected <%llu>\n", mlen1, mlen);
        return KAT_CRYPTO_FAILURE;
    }
        
    if ( memcmp(m, m1, mlen) ) {
        printf("crypto_sign_open returned bad 'm' value\n");
        return KAT_CRYPTO_FAILURE;
    }
	else{
		printf("Signature verification OK!\n");
	}*/
	
	// Print m1' into a file
	fp_m1 = fopen("m1.txt","w");
	fprintBstr(fp_sig, "m1 = ", m1, mlen1);
	fclose(fp_m1);
	
	///////////////////////////////////// signature verification form public key  ////////////////////////////////////////////////////////
    
	//free(m);
	free(m1);
	free(sm);

    return KAT_SUCCESS;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
FindMarker(FILE *infile, const char *marker)
{
	char	line[MAX_MARKER_LEN];
	int		i, len;
	int curr_line;

	len = (int)strlen(marker);
	if ( len > MAX_MARKER_LEN-1 )
		len = MAX_MARKER_LEN-1;

	for ( i=0; i<len; i++ )
	  {
	    curr_line = fgetc(infile);
	    line[i] = curr_line;
	    if (curr_line == EOF )
	      return 0;
	  }
	line[len] = '\0';

	while ( 1 ) {
		if ( !strncmp(line, marker, len) )
			return 1;

		for ( i=0; i<len-1; i++ )
			line[i] = line[i+1];
		curr_line = fgetc(infile);
		line[len-1] = curr_line;
		if (curr_line == EOF )
		    return 0;
		line[len] = '\0';
	}

	// shouldn't get here
	return 0;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
ReadHex(FILE *infile, unsigned char *A, int Length, char *str)
{
	int			i, ch, started;
	unsigned char	ich;
	int j;

	if ( Length == 0 ) {
		A[0] = 0x00;
		return 1;
	}
	memset(A, 0x00, Length);
	started = 0;
	if ( FindMarker(infile, str) ){
		j = 0;
		while ( (ch = fgetc(infile)) != EOF ) {
			if ( !isxdigit(ch) ) {
				if ( !started ) {
					if ( ch == '\n' )
						break;
					else
						continue;
				}
				else
					break;
			}
			started = 1;
			if ( (ch >= '0') && (ch <= '9') )
				ich = ch - '0';
			else if ( (ch >= 'A') && (ch <= 'F') )
				ich = ch - 'A' + 10;
			else if ( (ch >= 'a') && (ch <= 'f') )
				ich = ch - 'a' + 10;
            else // shouldn't ever get here
                ich = 0;
				
			printf("oi...\n");
			for ( i=0; i<Length-1; i++ )
				A[i] = (A[i] << 4) | (A[i+1] >> 4);
			A[Length-1] = (A[Length-1] << 4) | ich;
			j++;
			printf("%d\n",j);
		}
	}
	else
		return 0;

	return 1;
}

void
fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L)
{
	unsigned long long  i;

	fprintf(fp, "%s", S);

	for ( i=0; i<L; i++ )
		fprintf(fp, "%02X", A[i]);

	if ( L == 0 )
		fprintf(fp, "00");

	fprintf(fp, "\n");
}

