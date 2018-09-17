// gcc -std=c99 -fopenmp omp4.c -o omp4
// You fill in

#include <stdio.h>
#include <omp.h>

int main() {
	
	#pragma omp parallel
	{
	//	printf("This is universal to all threads\n");
		#pragma omp sections
		{
			printf("From now on this is run only once\n");
			#pragma omp section
			{
			printf("These need to be run");
			printf(" in sequence\n");
			}
			#pragma omp section
			{ printf("Now ending\n"); }
		}
	//printf("Now universal again\n");
	}

			

	return 0;
}
