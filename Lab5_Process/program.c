#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void child1Process(int pfd[]);
void child2Process(int pfd[]);
//void parentProcess(int pfd[]);
//char * child1[] = {"cd", "/proc", NULL};
//char * child2[] = {"ps", "-ef", NULL};
//char * parent[] = {"wc", "-l", NULL};

int main() {
	pid_t pid1, pid2;
	int pid;
	int child_status;

	int fd[2];
	pipe(fd);
	
	//int child_status_2;
	//int child_status_3;
	char * child1[] = {"/bin/whoami", NULL};
	char * child2[] = {"/bin/ls", "-l", NULL};
	char * parent[] = {"/bin/wc","-l", NULL};	
	/*	
	child1Process(fd);
	child2Process(fd);

	close(fd[0]);
	close(fd[1]);

	while ((pid = wait(&child_status)) != -1) {
		printf(stderr, "process %d exits with %d\n", pid, WEXITSTATUS(child_status));
		exit(0);
	}
	*/
	pid1 = fork();
	
	if (pid1 == 0) {
		printf("Child 1 running\n");
		execve(child1[0], child1, NULL);
		exit(0);
	}
	else {
		printf("Waiting for child 1 to finish\n");
		wait(&child_status);
		pid2 = fork();
		if (pid2 == 0) {
			printf("Child 2 running\n");
			dup2(fd[1], 1);
			close(fd[0]);
			execve(child2[0], child2, NULL);
			exit(0);
		}
		else {
			printf("Wait for child 2\n");
			wait(&child_status);
			printf("Execute parent now\n");
			dup2(fd[0], 0);
			close(fd[1]);
			execve(parent[0], parent, NULL);
		}
	}
	printf("end\n");
	return 0;
}
/*
void child1Process(int pfd[]) {
	int pid;
	switch (pid = fork()) {
		case 0: // child
			dup2(pfd[1], 1);
			close(pfd[0]);
			execve(child1[0], child1, NULL);
			exit(1);
			break;
		default: //parent
			break;
		case -1:
			perror("fork error\n");
			exit(1);
	}
}

void child2Process(int pfd[]) {
	int pid;

	switch (pid = fork()) {
                case 0: // child
                        dup2(pfd[0], 0);
                        dup2(pfd[1]; 1);
                        execve(child2[0], child2, NULL);
			perror(child2[0]);
                        exit(1);
                        break;
                default: //parent
                        dup2(pfd[1], 1);
			close(pfd[0]);
			execve(parent[0], parent, NULL);
			perror(parent[0]);
                case -1:
                        perror("fork error\n");
                        exit(1);
        }       

}

void parentProcess() {
	printf("parent running and ending\n");
} */
