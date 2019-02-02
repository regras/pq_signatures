#include <stdio.h>
#include <math.h>

void nu_t2w_calc(int *nu, int *nu_a, int *t2w, int *t2w_a, int *t2, int *t2_a, int *w, int *nc){

	*nu = (*t2)*(*w) - *nc;
	*t2w = (*t2)*(*w);
	
	*nu_a = (*t2_a)*(*w) - *nc;
	*t2w_a = (*t2_a)*(*w);

}
