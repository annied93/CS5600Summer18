#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

// function declaration:
int cd_func(char ** args);
int help_func(char ** args);
int exit_func(char ** args);
int pwd_func(char ** args);

// define variable flags
int background = 0;
int piping = 0;
int pipe_posn = 0;
int line_length = 0;
int left = 0;
int right = 0;
int posn_right = 0;
int posn_left = 0;
int terminated = 0;

char *builtin_name[] = {
  "cd",
  "help",
  "exit",
  "pwd"
};

int (*builtin_func[]) (char **) = {
  &cd_func,
  &help_func,
  &exit_func,
  &pwd_func
};

// for parsing
#define LINE_LENGTH 80
#define TOK_SIZE 64
#define DELIMITERS " \t\r\n\a"





int num_builtin() {
	return sizeof(builtin_name)/sizeof(char*);
}


int cd_func(char ** args) {
	if (args[1] == NULL) {
		fprintf(stderr, "please enter a directory for cd");
	}
	else if (strcmp("~", args[1]) == 0) {
		char *home = getenv("HOME");
		printf("%s\n", home);
		if (chdir(home) != 0) {
			perror("error");
	 	}
	 }
	else {
		if (chdir(args[1]) != 0) {
			perror("error");
		}
	}
	return 1;
}

int help_func(char ** args) {
	printf("Annie's Mini-shell\n");
  	printf("The following are built in:\n");
	int i;
  	for (i = 0; i < num_builtin(); i++) {
    	printf("  %s\n", builtin_name[i]);
  	}
  	printf("Use the man command for information on other programs.\n");
  	return 1;
}

int exit_func(char ** args) {
	printf("Now exiting... Goodbye!\n");
	return 0;
}

int pwd_func(char ** args) {
	char * env;
  	if((env = getenv("PWD")) == NULL) {
      		fprintf(stderr,"%s: Fail pwd \n",args[0]);
      	return -1;
  	}
  	else {
	      	printf("%s \n",env);
  	}
  	return 1;
}

void sigint_handler(int sig){
	write(1,"Mini-shell terminated\n",80);
	terminated = 1;
	exit(0);
}

char * read_line() {
	char * line = NULL;
	size_t size = LINE_LENGTH;
	size_t chars;
	line = (char*)malloc(size * sizeof(char));
	if (line == NULL) {
		perror("Allocation error");
		exit(1);
	}
	chars = getline(&line, &size, stdin);
	return line;
}

char * string_trim(char* line) {
	char * head = line;
	char * tail = line + strlen(line);
	while (*head == ' ') {
		head++;
	}
	while (*tail == ' ') {
		tail--;
	}
	*(tail+1) = '\0';
	return head;
}


char ** parse_line(char* line) {
	right = 0;
	left = 0;
	background = 0;
	piping = 0;
	int size = TOK_SIZE;
	int posn = 0;
	char ** tokens = malloc(size * sizeof(char*));
	char * token;



	if (!tokens) {
		fprintf(stderr, "Allocation error");
		exit(EXIT_FAILURE);
	}
	line = string_trim(line);


		token = strtok(line, DELIMITERS);
  		while (token != NULL) {
  			if (strcmp("&", token) == 0) {
  				background = 1;
  				token = NULL;
				tokens[posn] = token;
				line_length = posn;
				return tokens;
  			}
  			if (strcmp("|", token) == 0) {
  				piping = 1;
  				pipe_posn = posn;
  			}
  			if (strcmp(">", token) == 0) {
  				right = 1;
  				posn_right = posn;
  			}
  			if (strcmp("<", token) == 0) {
  				left = 1;
  				posn_left = posn;
  			}
    	tokens[posn] = token;
    	posn++;
    	token = strtok(NULL, DELIMITERS);
  	}

  	tokens[posn] = NULL;
  	line_length = posn;
  	return tokens;
	
}

// void runpipe(int pipeline[], char** func1, char** func2) {
// 	int pid;
// 	int child_status;
// 	printf("in fn\n");


// }
void redirect_write(char ** args);
void redirect_read(char ** args);

int launch_pipe(char ** args) {
//	piping = 0;
	int pid, pid2;
	int pipeline[2];
	int status;
	int child_status;
	char ** left_cmd = malloc(LINE_LENGTH * sizeof(char*));
	if (left_cmd == NULL) {
		perror("Allocation error");
		exit(EXIT_FAILURE);
	}
	char ** right_cmd = malloc(LINE_LENGTH * sizeof(char*));
	if (right_cmd == NULL) {
		perror("Allocation error");
		exit(EXIT_FAILURE);
	}
	
	pipe(pipeline);

	int i;
	for (i = 0; i < pipe_posn; i++) {
		printf("Args %d: %s\n", i, args[i]);
		left_cmd[i] = args[i];
		printf("Left %d: %s\n", i, left_cmd[i]);
	}
	left_cmd[i] = NULL;
	int j = 0;
	for (i = pipe_posn + 1; i < line_length; i++) {
		printf("Args %d: %s\n", i, args[i]);
		right_cmd[j] = args[i];
		printf("Right %d: %s\n", j, right_cmd[j]);
		j++;
	}
	right_cmd[j] = NULL;


	pid = fork();
	if (pid < 0) {
		perror("error forking: pid");
		//exit(EXIT_FAILURE);
	}
	else if (pid == 0) {
	// child1
		pid2 = fork();
		if (pid2 == 0) {
		//child2
			//printf("right function\n");
    			dup2(pipeline[0], 0); // input
    			close(pipeline[1]);
			//close(pipeline[1]);
			//printf("before running command\n");
			execvp(right_cmd[0], right_cmd);
			perror(right_cmd[0]);
			
		//pid2 = fork();
		}
		else if (pid2 < 0 ) {
			perror("fork: pid2");
			//exit(EXIT_FAILURE);
		}
		else {
			//child2 parent
			//printf("lefunction\n");
			//waitpid(pid2, NULL, 0);
			//do {
			//	waitpid(pid2, &child_status, WUNTRACED);
			//} while (!WIFEXITED(status) && !WIFSIGNALED(status));
    			//child
    			dup2(pipeline[1], 1); // standard input
    			close(pipeline[0]);
			//close(pipeline[0]);
			//printf("hahahaha\n");
    			execvp(left_cmd[0], left_cmd);
			printf("should not be here\n");
    			perror(left_cmd[0]);
                	//close(pipeline[0]);
			//close(pipeline[1]);
			
			waitpid(pid2, NULL, 0);
		}
                //perror(left_cmd[0]);
                //
			//printf("right function\n");
			//wait(&child_status);
    			//child
    			//dup2(pipeline[0], 0); // standard input
    			//close(pipeline[1]);
			//printf("hahahaha\n");
    			//execvp(right_cmd[0], right_cmd);
			//printf("should not be here\n");
    			//perror(right_cmd[0]);
		
		//close(pipeline[0]);
		//close(pipeline[1]);
		//printf("finish the baby\n");
	}
	else {
		// parent of child1
		//printf("back to parent of child1\n");
		if (background == 0) {
		do {
      			//printf("waiting for process in child to finish\n");
			waitpid(pid, &status, WUNTRACED);
    		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	else {
		//background == 0;
		signal(SIGCHLD, SIG_IGN);
	}
	}
	return EXIT_SUCCESS;
}

void redirect_write(char** args) {
	int fd;
	char * output = args[posn_right+1];
	args[posn_right] = NULL;
	if((fd = open(output, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0 ) {
		perror(args[0]);
		exit(EXIT_FAILURE);
	}
	
	dup2(fd,STDOUT_FILENO); // become standard output
	//close(fd);
	//printf("finish\n");
	if (execvp(args[0], args) == -1) {
		perror(args[0]);
		exit(EXIT_FAILURE);
	}
	close(fd);
	printf("close fd successful\n");
	//fclose(output);
}	

void redirect_read(char** args) {
        int fd;
        char * input = args[posn_left+1];
        args[posn_left] = NULL;
        if((fd = open(input, O_CREAT | O_RDONLY, 0644)) < 0 ) {
                perror(args[0]);
                exit(EXIT_FAILURE);
        }
        
        dup2(fd,STDIN_FILENO); // become standard output
        //printf("finish\n");
        if (execvp(args[0], args) == -1) {
                perror(args[0]);
                exit(EXIT_FAILURE);
        }
        close(fd);
}


int launch(char ** args) {
	pid_t pid;
	int size = TOK_SIZE;
	int pipeid;
	int status;
	int fd0, fd1;
  	
	pid = fork();
	if (pid < 0) {
    	// Error forking
    		perror("fork");
    		exit(EXIT_FAILURE);
  	}
  	else if (pid == 0) {
  		if (right == 1) {
  		
			printf("right flag: %d\n", right);
			redirect_write(args);
			//if((fd0 = open(output, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0 ) {
			//	perror(args[0]);
			//	exit(EXIT_FAILURE);
			//}
			fflush(stdout);
			
  			//dup2(fd0, 1); // become standard output
  			//printf("finish\n");
  			//if (execvp(args[0], args) == -1) {
			//	perror(args[0]);
			//	exit(EXIT_FAILURE);
			//}
			//close(fd0);
  			//printf("Finish writing\n");
  			//fflush(stdout);
			//right = 0;
  		}
  		else if (left == 1) {
			//char * input = args[posn_left+1];
			//args[posn_left] = NULL;
  			//if((fd1 = open(input, O_RDONLY | O_CREAT, 0644)) < 0) {
			//	perror(args[0]);
			//	exit(EXIT_FAILURE);
			//}
  			//dup2(fd1, 0);
    			//if (execvp(args[0], args) == -1) {
    			//	perror("error executing");
    			//	exit(EXIT_FAILURE);
    			//}
			//close(fd1);
			//left = 0;
			printf("Left: %d\n", left);
			redirect_read(args);
		}
		else {
			if (execvp(args[0], args) == -1) {
				perror(args[0]);
				exit(EXIT_FAILURE);
			}
		}
    	}
  	else {
//		if (right == 1) {
//                        right = 0;
//                }
//                if (left == 1) {
//                        left = 0;
//                }

    	// Parent process
    	if (background == 0) {
//		if (right == 1) {
//			right = 0;
//		}
//		if (left == 1) {
//			left = 0;
//		}
    		//printf("back to parent\n");
    		do {
      			waitpid(pid, &status, WUNTRACED);
    		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
    	}
    	else {
 //   		background = 0;
		signal(SIGCHLD, SIG_IGN);
    	}
 	}
 	fflush(stdout);
  	return 1;
}


int execute(char** args) {
	if (args[0] == NULL) {
		return 1;
	}
	int i;
	for (i = 0; i < num_builtin(); i++) {
		if (strcmp(args[0], builtin_name[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}
	if (piping == 1) {
		return launch_pipe(args);
	}
	return launch(args);
}

void shell_init() {
	struct sigaction sigint_a = {
		.sa_handler = &sigint_handler,
		.sa_flags = 0
	};
	sigemptyset(&sigint_a.sa_mask);
	sigaction(SIGINT, &sigint_a, NULL);

	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);

}


void shell_loop() {
	char * line;
	char ** args;
	int status;

	do {
		printf("mini-shell>");
		line = read_line();
		args = parse_line(line);
		status = execute(args);
		free(line);
		free(args);
	} while (status);
}



int main(int argc, char** argv) {
	
	printf("You can terminate by pressing Ctrl+C\n");
	shell_init();	
	if (terminated == 1) {
		return EXIT_SUCCESS;
	}
	shell_loop();

	return EXIT_SUCCESS;
}

