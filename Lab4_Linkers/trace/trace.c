#include <stdio.h>
#include <time.h>

int count = 0;
struct timespec start_t, end_t;
double start, end, diff_t;

__attribute__((no_instrument_function))
void __cyg_profile_func_enter(void *this_fn, void *call_site){
	
	printf("Function entered\n");
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start_t); 
	count++;
	printf("Function execution: %d\n", count);

	//time(&end_t);
	//diff_t = difftime(end_t, start_t);
	//printf("time: %f\n", diff_t);
}

__attribute__((no_instrument_function))
void __cyg_profile_func_exit(void *this_fn, void *call_site){

	printf("Function exited\n");
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_t);
	start = 1000000000 * start_t.tv_sec + start_t.tv_nsec;
	end = 1000000000 * end_t.tv_sec + end_t.tv_nsec;
	diff_t = end - start;

	printf("Execution time: %f\n", diff_t);
}

