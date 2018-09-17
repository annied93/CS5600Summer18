#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // This is new!


// Create a signal handler
void sigint_handler(int sig){
	write(1,"Terminating through signal handler\n",80);
	exit(0);
}

int main(){
	// Install our signal handler
	signal(SIGINT, sigint_handler);

 	printf("You can only terminate by pressing Ctrl+C\n");

	while(1){
		printf("Running forever!\n");
		sleep(1);
	}

	return 0;
}
