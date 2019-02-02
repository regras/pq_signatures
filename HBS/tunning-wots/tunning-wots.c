#include <stdio.h>
#include <math.h>
#include "functions.h"

#define W 8
#define M 4

/* This program prints a table for a set of WOTS parameters.*/

int main(){

	FILE *fp;

	int w_array[W] = {2,4,6,8,10,12,14,16};
	int m_array[M] = {128,192,256,512};

	int t1[W][M];
	int t2[W][M];
	int nc[W][M];
	int nu[W][M];
	int t2w[W][M];

	int *pW;
	int *pM;

	int *pT1;
	int *pT2;
	int *pNc;
	int *pNu;
	int *pT2w;

	int i,j;

	pW = w_array;
	pM = m_array;

	pT1 = t1[0];
	pT2 = t2[0];
	pNc = nc[0];
	pNu = nu[0];
	pT2w = t2w[0];

	fp = fopen("table1.txt","w");

	for(i=0; i<W; i++){
		pM = m_array;
		for(j=0; j<M; j++){
			t1_calc(pM, pW, pT1);
			t2_nc_calc(pT1, pW, pT2, pNc);
			nu_t2w_calc(pNu, pT2w, pT2, pW, pNc);
			pM++;
			pT1++;
			pT2++;
			pNc++;
			pNu++;
			pT2w++;
		}
		pW++;
	}

	pW = w_array;
	for(i=0; i<W; i++){
		pM = m_array;
		for(j=0; j<M; j++){
			fprintf(fp, "w = %d m = %d nc = %d nu = %d t2w = %d \n", *pW, *pM, nc[i][j], nu[i][j], t2w[i][j]);
			pM++;
		}
		pW++;
		fprintf(fp, "\n");
	}
	
	for(i=0; i<W; i++){
		for(j=0; j<M; j++){
			printf("t2 = %d ", t2[i][j]);
		}
		printf("\n\n");
	}

	fclose(fp);

	return 0;
}
