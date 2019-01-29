#include <stdio.h>

unsigned char
HexToChar(unsigned char ch){
	
	unsigned char ich;
	
	if ( (ch >= '0') && (ch <= '9') )
		ich = ch - '0';
	else if ( (ch >= 'A') && (ch <= 'F') )
		ich = ch - 'A' + 10;
	else if ( (ch >= 'a') && (ch <= 'f') )
		ich = ch - 'a' + 10;
    else // shouldn't ever get here
        ich = 0;
	
	return ich;
}

unsigned char
CharValue(unsigned char ch, unsigned char ch1){
	
	unsigned char char_value;
	
	char_value = 16*ch + ch1;
	
	return char_value;
	
}