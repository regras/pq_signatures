#include <stdio.h>
#include <math.h>

/* As there is no log2(x) in the C language, log2(x) = log10(x)/log10(2). 

t2 = ceil( ( floor( log2(t1) ) + 1 + w ) /w )
but t2 = ceil( ( log2(t1) + w ) /w ), if t1 is power of two.

*/


void t2_nc_calc(int *t1, int *w, int *t2, int *nc){

	//number of bits to represent the integer t1.
	double n;
	
	double aux;

	if( *t1 == 2 || *t1 == 4 || *t1 == 8 || *t1 == 16 || *t1 == 32 || *t1 == 64 || *t1 == 128 || *t1 == 256 )
		n = log10( (double)*t1 ) / log10(2);
	else
		n = floor( log10( (double)*t1) / log10(2) ) + 1;
	
	aux =  ceil((n+(*w))/(*w));

	*t2 = (int) ceil( (n + (*w) ) / (*w) );
	*nc = (int) n + (*w);
	
	
	printf("t1 = %d t2 = %d n = %f aux = %f \n", *t1, *t2, n, aux);

}
