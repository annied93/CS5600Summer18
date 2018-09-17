// gcc -std=c99 -fopenmp omp5.c -o omp5
// You fill in
//
#include <stdio.h>
#include <omp.h>
#include <math.h>




int main() {
	const int size = 10;
	


	#pragma omp for schedule(dynamic, 5)
	for (int n = 0; n < size; ++n) {
		printf("   %d", n);
	}	
	printf(".\n");
		
	return 0;
}
