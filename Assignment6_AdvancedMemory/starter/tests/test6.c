#include <stdio.h>
#include "malloc.h"
#include <pthread.h>


#define NTHREADS 32
pthread_mutex_t mutex;
int counter = 0;
void* test_malloc(void* vargp) {
	printf("Thread %ld running\n", pthread_self());
	++counter;
	printf("Counter value: %d\n", counter);
	int* ptr = (int*)malloc(1030);
	printf("pointer just allocated is %p\n", ptr);
//	--counter;
	printf("Counter value: %d\n", counter);
	printf("Pointer to free is %p\n", ptr);
	free(ptr);
}


int main(int argc, char** argv) {
	pthread_t thread_ids[NTHREADS];
	for (int i = 0; i < NTHREADS; ++i) {
		pthread_create(&thread_ids[i], NULL, test_malloc, NULL);
	}
	for (int i = 0; i < NTHREADS; ++i) {
		pthread_join(thread_ids[i], NULL);
	}
	return 0;
}
