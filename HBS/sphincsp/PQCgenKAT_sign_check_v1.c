
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
    char 		               	fn_rsp[32];
    FILE                		*fp_rsp;
    unsigned char       	msg[3300];
    unsigned char       	*m, *sm, *m1;
    unsigned long long  mlen, smlen, mlen1;
    int                 		done;
    unsigned char       	pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES];
    int                 		ret_val;
    
    // Open REQUEST file, where msg, sk, pk and sig values are printed.
    sprintf(fn_rsp, "PQCsignKAT_%d.rsp", CRYPTO_SECRETKEYBYTES);
    if ( (fp_rsp = fopen(fn_rsp, "r")) == NULL ) {
        printf("Couldn't open <%s> for read\n", fn_rsp);
        return KAT_FILE_OPEN_ERROR;
    }
    
	// Reading values from file.
	
	// mlen (mensage size)
	if (FindMarker(fp_rsp, "mlen = ") )
         fscanf(fp_rsp, "%llu", &mlen);
	else
		printf("ERROR: unable to read 'mlen' from <%s>\n", fn_rsp);
	
	// Message
	m = (unsigned char *)calloc(mlen, sizeof(unsigned char));
    if ( !ReadHex(fp_rsp, m, (int)mlen, "msg = ") ) {
         printf("ERROR: unable to read 'msg' from <%s>\n", fn_rsp);
         return KAT_DATA_ERROR;
    }
	
	// Public key
	if ( !ReadHex(fp_rsp, pk, CRYPTO_PUBLICKEYBYTES, "pk = ") ) {
         printf("ERROR: unable to read 'pk' from <%s>\n", fn_rsp);
         return KAT_DATA_ERROR;
    }
	
	// Secret key
	if ( !ReadHex(fp_rsp, sk, CRYPTO_SECRETKEYBYTES, "sk = ") ) {
         printf("ERROR: unable to read 'sk' from <%s>\n", fn_rsp);
         return KAT_DATA_ERROR;
    }
	else
		printf("sk ok!\n");
	
	// smlen (signature size)
	if(FindMarker(fp_rsp, "smlen = ") )
         fscanf(fp_rsp, "%llu", &smlen);
	else
		printf("ERROR: unable to read 'mlen' from <%s>\n", fn_rsp);
	
	 // Signature
	 //sm = (unsigned char *)calloc(smlen, sizeof(unsigned char));
	 sm = (unsigned char *)calloc(mlen+CRYPTO_BYTES, sizeof(unsigned char));
	if ( !ReadHex(fp_rsp, sm, smlen, "sm = ") ) {
         printf("ERROR: unable to read 'sm' from <%s>\n", fn_rsp);
         return KAT_DATA_ERROR;
    }
	 
	// Signature calculation
	if ( (ret_val = crypto_sign(sm, &smlen, m, mlen, sk)) != 0) {
         printf("crypto_sign returned <%d>\n", ret_val);
         return KAT_CRYPTO_FAILURE;
    }
        
    // Signature verification form public key
	m1 = (unsigned char *)calloc(mlen+CRYPTO_BYTES, sizeof(unsigned char));
	if ( (ret_val = crypto_sign_open(m1, &mlen1, sm, smlen, pk)) != 0) {
        printf("crypto_sign_open returned <%d>\n", ret_val);
        return KAT_CRYPTO_FAILURE;
    }
        
    if ( mlen != mlen1 ) {
        printf("crypto_sign_open returned bad 'mlen': Got <%llu>, expected <%llu>\n", mlen1, mlen);
        return KAT_CRYPTO_FAILURE;
    }
        
    if ( memcmp(m, m1, mlen) ) {
        printf("crypto_sign_open returned bad 'm' value\n");
        return KAT_CRYPTO_FAILURE;
    }
	else{
		printf("Signature verification OK!\n");
	}
	
    free(m);
	free(m1);
	free(sm);
	
	fclose(fp_rsp);

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

	if ( Length == 0 ) {
		A[0] = 0x00;
		return 1;
	}
	memset(A, 0x00, Length);
	started = 0;
	if ( FindMarker(infile, str) )
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
			
			for ( i=0; i<Length-1; i++ )
				A[i] = (A[i] << 4) | (A[i+1] >> 4);
			A[Length-1] = (A[Length-1] << 4) | ich;
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

