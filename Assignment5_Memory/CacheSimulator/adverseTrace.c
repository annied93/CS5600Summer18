#include <stdio.h>
#include <stdlib.h>
#include "binary.h"


#define BITWIDTH 16	// Our architecture is an 16-bit architecture
#define ADDRESSES 16384	// We will generate 16384 instructions
#define RANGE 16384	// The range of these instructions is a value 0-RANGE


void adverseTrace(int size, int range) {
	FILE* fp = fopen("adverseTrace.txt","w");

	for(int i=0; i < size; i++){
		char* bin=intToBinaryString((i%range),BITWIDTH);
		fprintf(fp,"%s\n",bin);
		free(bin);
	}

	fclose(fp);
}

int main() {
	adverseTrace(ADDRESSES, RANGE);
}
