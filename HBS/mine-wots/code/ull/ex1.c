#include <stdio.h>

int main(){
	int num = 8;
	for(int i =0; i< 4; i++){
		printf("%d\n",num);
		num = num << 2;
	}
	
	return 1;
}