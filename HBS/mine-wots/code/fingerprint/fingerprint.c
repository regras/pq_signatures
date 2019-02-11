#include <stdio.h>

#include "../../params.h"
#include "../../read.h"
#include "../../utils.h"


/**
 * Print on screen the fingerprint of the a message's digest.
 */

static void base_w(const xmss_params *params,
                   int *output, const int out_len, const unsigned char *input);
static void wots_checksum(const xmss_params *params,
                          int *csum_base_w, const int *msg_base_w);
static void chain_lengths(const xmss_params *params,
                          int *lengths, const unsigned char *msg);

int main(){
	
	FILE *fp;
	
	int j;
	
	xmss_params params;
    uint32_t oid = 0x00000001;
	xmss_parse_oid(&params, oid);
	
	xmss_params *params1;
	
	fp = fopen("values.txt","r");
	
	unsigned char seed[params.n];
	unsigned char m[params.n];
	int lengths[params.wots_len];
	
	params1 = &params;
	
	printf("Oi\n");
	
	// seed read from a file
	if ( !ReadHex(fp, seed, 32, "seed = ") ) {
         printf("ERROR: unable to read 'seed' from values.txt\n");
    }
	
	for(j=0; j<32; j++)
		printf("%02X\n",seed[j]);
	
	// read m from a file
	if ( !ReadHex(fp, m, 32, "m = ") ) {
         printf("ERROR: unable to read 'seed' from values.txt\n");
    }
	
	for(j=0; j<32; j++)
		printf("%02X\n",m[j]);
	
	chain_lengths(params1, lengths, m);
	
	printf("Fingerprint...\n");
	
	for (j = 0; j < (int)params.wots_len; j++) {
		printf("%d\n",lengths[j]);
	}
	
	fclose(fp);
}

/**
 * base_w algorithm as described in draft.
 * Interprets an array of bytes as integers in base w.
 * This only works when log_w is a divisor of 8.
 */
static void base_w(const xmss_params *params,
                   int *output, const int out_len, const unsigned char *input)
{
    int in = 0;
    int out = 0;
    unsigned char total;
    int bits = 0;
    int consumed;

    for (consumed = 0; consumed < out_len; consumed++) {
        if (bits == 0) {
            total = input[in];
            in++;
            bits += 8;
        }
        bits -= params->wots_log_w;
        output[out] = (total >> bits) & (params->wots_w - 1);
        out++;
    }
}

/* Computes the WOTS+ checksum over a message (in base_w). */
static void wots_checksum(const xmss_params *params,
                          int *csum_base_w, const int *msg_base_w)
{
    int csum = 0;
    unsigned char csum_bytes[(params->wots_len2 * params->wots_log_w + 7) / 8];
    unsigned int i;

    /* Compute checksum. */
    for (i = 0; i < params->wots_len1; i++) {
        csum += params->wots_w - 1 - msg_base_w[i];
    }

    /* Convert checksum to base_w. */
    /* Make sure expected empty zero bits are the least significant bits. */
    csum = csum << (8 - ((params->wots_len2 * params->wots_log_w) % 8));
    ull_to_bytes(csum_bytes, sizeof(csum_bytes), csum);
    base_w(params, csum_base_w, params->wots_len2, csum_bytes);
}

/* Takes a message and derives the matching chain lengths. */
static void chain_lengths(const xmss_params *params,
                          int *lengths, const unsigned char *msg)
{
    base_w(params, lengths, params->wots_len1, msg);
    wots_checksum(params, lengths + params->wots_len1, lengths);
}