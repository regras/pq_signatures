#include <stdio.h>
#include <math.h>

void nu_t2w_calc(int *nu, int *t2w, int *t2, int *w, int *nc){

	*nu = (*t2)*(*w) - *nc;
	*t2w = (*t2)*(*w);

}
