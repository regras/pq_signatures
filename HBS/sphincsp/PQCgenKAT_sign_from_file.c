
//
//  PQCgenKAT_sign_from_file.c
//
// Created by Antônio Unias on 1/25/19.
// This file obtains seed, message (msg) and secret key(sk) from files 
// seed.txt, msg.txt and sk.txt, respectively. Then, a signature is generate
// and printed in a file (sm_output.txt).
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rng.h"
#include "api.h"
#include "constants.h"
#include "reading.h"
#include "interface.h"

#define	MAX_MARKER_LEN		50

#define KAT_SUCCESS          0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_DATA_ERROR      -3
#define KAT_CRYPTO_FAILURE  -4

int		FindMarker(FILE *infile, const char *marker);
int		ReadHex(FILE *infile, unsigned char *A, int Length, char *str);
void	fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L);

char    AlgName[] = "My Alg Name";

int
main()
{
    FILE *fp_seed, *fp_msg, *fp_sk;
	FILE *fp_sig;
	
	unsigned char seed[SEED_NBYTES];
	unsigned char msg[MSG_NBYTES];
	unsigned char sk[SK_NBYTES];
	
	unsigned char	*p_m, *p_sk;
    unsigned char    *sm;
    unsigned long long  mlen, smlen;
    
	int                 ret_val;
    
    // Reading data from file.
	fp_seed = fopen("seed.txt","r");
	fp_msg = fopen("msg.txt","r");
	fp_sk = fopen("sk.txt","r");
	
	// File to store the SPHINCS+ signature.
	fp_sig = fopen("sig_output.txt","w");
	
	// Reading the values of seed, msg and sk from files.
	FileReading(fp_seed, SEED_NBYTES, seed);
	FileReading(fp_msg, MSG_NBYTES, msg);
	FileReading(fp_sk, SK_NBYTES, sk);
	
	fclose(fp_seed);
	fclose(fp_msg);
	fclose(fp_sk);
	
	// Printing data on the screen to check it is ok.
	for(int i=0;i<SEED_NBYTES;i++){
		printf("-- %hhu %02x\n",seed[i],seed[i]);
	}
	
	printf("\n\n");
	
	for(int i=0;i<MSG_NBYTES;i++){
		printf("-- %hhu %02x\n",msg[i],msg[i]);
	}
	
	printf("\n\n");
	
	for(int i=0;i<SK_NBYTES;i++){
		printf("-- %hhu %02x\n",sk[i],sk[i]);
	}
	
	p_m = &msg;
	p_sk = &sk;
	
	mlen = SIG_NBYTES;
	
	// Allocating memory for sm (signature)
	sm = (unsigned char *)calloc(mlen+CRYPTO_BYTES, sizeof(unsigned char));
	
	printf("Alocou memória.\n");
	
	// Signature generation
	ret_val = crypto_sign(sm, &smlen, p_m, mlen, p_sk);
	
	printf("Valor de smlen = %llu\n",smlen);
	
	// Printing the signature into a file.
	for(int i =0;i<SIG_NBYTES;i++)
		fprintf(fp_sig,"%02X",sm[i]);
	
	/*
    done = 0;
    do {
                
        //m = (unsigned char *)calloc(mlen, sizeof(unsigned char));
        //m1 = (unsigned char *)calloc(mlen+CRYPTO_BYTES, sizeof(unsigned char));
        sm = (unsigned char *)calloc(mlen+CRYPTO_BYTES, sizeof(unsigned char));
        
        
        
		// Generate the public/private keypair
        if ( (ret_val = crypto_sign_keypair(pk, sk)) != 0) {
            printf("crypto_sign_keypair returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
        }
        fprintBstr(fp_rsp, "pk = ", pk, CRYPTO_PUBLICKEYBYTES);
        fprintBstr(fp_rsp, "sk = ", sk, CRYPTO_SECRETKEYBYTES);
        	
		
        if ( (ret_val = crypto_sign(sm, &smlen, m, mlen, sk)) != 0) {
            printf("crypto_sign returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
        }
        fprintf(fp_rsp, "smlen = %llu\n", smlen);
        fprintBstr(fp_rsp, "sm = ", sm, smlen);
        fprintf(fp_rsp, "\n");
        
        if ( (ret_val = crypto_sign_open(m1, &mlen1, sm, smlen, pk)) != 0) {
            printf("crypto_sign_open returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
        
        
        if ( mlen != mlen1 ) {
            printf("crypto_sign_open returned bad 'mlen': Got <%llu>, expected <%llu>\n", mlen1, mlen);
            return KAT_CRYPTO_FAILURE;
        }
        
        if ( memcmp(m, m1, mlen) ) {
            printf("crypto_sign_open returned bad 'm' value\n");
            return KAT_CRYPTO_FAILURE;
        }
        
        //free(m);
        //free(m1);
        //free(sm);

    } while ( !done );
    */
    
	free(sm);
	fclose(fp_sig);
	
	return ret_val;

    //return KAT_SUCCESS;
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

