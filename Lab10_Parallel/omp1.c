// gcc -std=c99 -fopenmp omp1.c -o omp1
// You fill in
//

#include <stdio.h>
#include <omp.h>

int main() {
	#pragma omp parallel num_threads(3)
	{
		#pragma omp for
		for (int n = 0; n < 10; ++n) {
			printf("Thread number: %d\n", n);
		}
	}
	return 0;
}
