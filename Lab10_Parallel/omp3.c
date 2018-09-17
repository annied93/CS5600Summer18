// gcc -std=c99 -fopenmp omp3.c -o omp3
// You fill in
//

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

//int b = 10;

int main(int argc, char** argv) {
	//int b = 10;
	int r = 0;

	#pragma omp target if(atoi(argv[1])) map(tofrom:r) 

	r += 4;
	printf("r = %d\n", r);
	

	return 0;
}
	
