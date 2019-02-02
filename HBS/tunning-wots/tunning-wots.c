#include <stdio.h>
#include <math.h>
#include "functions.h"

#define W 8
#define M 4

/* This program prints a table for a set of WOTS parameters.*/

/* 
Using the formula t2 = ceil( ( floor( log2(t1) ) + 1 + w ) /w ) for all values of t1 is incorrect.
If t1 is power of two, t2 = ceil( ( log2(t1) + w ) /w ).
*/

int main(){

	FILE *fp;

	int w_array[W] = {2,4,6,8,10,12,14,16};
	int m_array[M] = {128,192,256,512};

	int t1[W][M];
	int t2[W][M];
	int nc[W][M];
	int nu[W][M];
	int t2w[W][M];
	
	int t2_a[W][M];
	int nu_a[W][M];
	int t2w_a[W][M];

	int *pW;
	int *pM;

	int *pT1;
	int *pT2;
	int *pNc;
	int *pNu;
	int *pT2w;
	
	int *pT2_a;
	int *pNu_a;
	int *pT2w_a;

	int i,j;

	pW = w_array;
	pM = m_array;

	pT1 = t1[0];
	pT2 = t2[0];
	pNc = nc[0];
	pNu = nu[0];
	pT2w = t2w[0];
	
	pT2_a = t2_a[0];
	pNu_a = nu_a[0];
	pT2w_a = t2w_a[0];

	fp = fopen("table1.txt","w");

	for(i=0; i<W; i++){
		pM = m_array;
		for(j=0; j<M; j++){
			t1_calc(pM, pW, pT1);
			t2_nc_calc(pT1, pW, pT2, pT2_a, pNc);
			nu_t2w_calc(pNu, pNu_a, pT2w, pT2w_a, pT2, pT2_a, pW, pNc);
			pM++;
			pT1++;
			pT2++;
			pNc++;
			pNu++;
			pT2w++;
			pT2_a++;
			pNu_a++;
			pT2w_a++;
		}
		pW++;
	}

	pW = w_array;
	for(i=0; i<W; i++){
		pM = m_array;
		for(j=0; j<M; j++){
			fprintf(fp, "w = %5d  m = %5d  t1 = %5d  t2 = %5d  t2' = %5d  nc = %5d  nu = %5d  nu' = %5d  t2w = %5d  t2w' = %5d\n", *pW, *pM, t1[i][j], t2[i][j], t2_a[i][j], nc[i][j], nu[i][j], nu_a[i][j], t2w[i][j], t2w_a[i][j]);
			pM++;
		}
		pW++;
		fprintf(fp, "\n");
	}
	
	for(i=0; i<W; i++){
		for(j=0; j<M; j++){
			printf("t2 = %d t2' = %d ", t2[i][j], t2_a[i][j]);
		}
		printf("\n\n");
	}

	fclose(fp);

	return 0;
}
