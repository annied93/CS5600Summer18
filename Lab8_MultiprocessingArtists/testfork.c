// fork.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// Add your paint function here


int main(int argc, char** argv){
	int numberOfArtists = 8;
	pid_t wpid;
	int status;
	
	int *arr = (int*)malloc(sizeof(int) * numberOfArtists);
	for(int i =0; i < numberOfArtists; i++){
		int pid = fork();
		// (2) Make only the child do some work
		if (pid == 0) {
			printf("Child doing work\n");
			exit(0);
		}
		else if (pid > 0) {
			while ((wpid = wait(&status)) > 0) {
			//	waitpid(pid, 0, NULL);
			// (3) Print out the child pid (from the parent--mostly for logging purposes)
			printf("Child pid is %d\n", pid);
			}
		}
	}
	printf("parent is exiting(last artist out!)\n");
	free(arr);
	return 0;
}
