#include <stdio.h>
#include <stdlib.h>
#include "binary.h"

#define BITWIDTH 16	// Our architecture is an 16-bit architecture
#define ADDRESSES 16384	// We will generate 16384 instructions
#define RANGE 16384	// The range of these instructions is a value 0-RANGE


void friendlyTrace(int size, int range){
	FILE* fp = fopen("realistic.txt","w");

        srand(time(NULL));


        for(int i=0; i < size;){

              

                        char* bin1 = intToBinaryString((rand()%range),BITWIDTH);
                        char* bin2 = intToBinaryString((rand()%range),BITWIDTH);
                        char* bin3 = intToBinaryString((rand()%range),BITWIDTH);

                        for(int j = 0; j < 10 && i < size; j++, i+=3){
                                fprintf(fp,"%s\n",bin1);
                                fprintf(fp,"%s\n",bin2);
                                fprintf(fp,"%s\n",bin3);
                                //i+=3;
                        }

                        free(bin1);
                        free(bin2);
                        free(bin3);
                
        }

        fclose(fp);
}

int main() {
	friendlyTrace(ADDRESSES, RANGE);
}
