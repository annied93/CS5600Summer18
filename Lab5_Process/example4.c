#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

    int child_status;

    if(fork()==0){
    	printf("Child runs!\n");
	exit(0); // and exits properly
    }else{
	printf("Hello from parent\n");
	wait(&child_status);
	printf("Child always is terminated by this point\n");
    }

	printf("This always prints last\n");

	return 0;
}
