#include <stdio.h>
#include <string.h>
#include "interface.h"

void FileReading(FILE *fp, int size, unsigned char value[size]){
	
	unsigned char ch, ch1, ich;
	
	unsigned char hex_value[2*size+1];
	unsigned char value_aux[size+1];
	
	int j;
	
	int len;
	
	j = 0;
	while(!feof(fp)){
		fscanf(fp,"%s'",hex_value);
		len = strlen(hex_value);
		//printf("Entrou.\n");
		//printf("%s  e %d e size: %d\n",hex_value, len, size);
		
		for(int i=0;i<2*size;i=i+2){
			ch = HexToChar(hex_value[i]);
			ch1 = HexToChar(hex_value[i+1]);
			ich = CharValue(ch,ch1);
			value_aux[j] = ich;
			//printf("%hhu %02x\n",value_aux[j],value_aux[j]);
			j++;
		}
	
	}
	
	//printf("Saiu loop\n");
	
	memcpy (value, value_aux, size);
	
	//for(int i =0; i<3; i++)
	//	printf("V %hhu\n",value[i]);
	
	//fclose(fp);
	
	//printf("Saiu!\n");
}

