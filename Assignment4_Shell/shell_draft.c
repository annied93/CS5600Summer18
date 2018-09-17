// My shell script

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <glob.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>


// CONSTANTS & DATA STRUCTURES

// number of jobs
#define NUM_JOBS 10
// line buffer size = 80
#define LINE_SIZE 80
#define PATH_SIZE 1024
// token size and delimiters
#define TOKEN_SIZE 80 // token size is the same length as max line length, no need to realloc
#define DELIMITERS " \t\r\n\a"

// filter flags
#define PROC_FILTER_ALL 0
#define PROC_FILTER_DONE 1
#define PROC_FILTER_REMAIN 2

// status flags
#define STATUS_RUNNING 0
#define STATUS_DONE 1
#define STATUS_SUSPENDED 2
#define STATUS_CONTINUED 3
#define STATUS_TERMINATED 4

// command flags
#define NUM_BUILTINS 4
#define COMMAND_EXTERNAL 0
#define COMMAND_EXIT 1
#define COMMAND_CD 2
#define COMMAND_HELP 3
#define COMMAND_PWD 4

// mode flags
#define BACKGROUND 0
#define FOREGROUND 1
#define PIPELINE 2

const char * BUILTIN_STR[] = {
	"exit"
	"cd",
	"help",
	"pwd",
	
};

// status-string array
const char * STATUS_STRING[] = {
	"running",
	"done",
	"suspended",
	"continued",
	"terminated"
};

// process struct ~ represent a single process
struct process {
	char* command;
	int argc;
	char ** argv;
	char * in_path; // input path
	char * out_path; //output path
	pid_t pid;
	int type;
	int status;
	struct process *next;
};

// job struct ~ a pipeline of process: only need to handle 2
struct job {
	int id;
	struct process *root;
	char * command; 
	pid_t pgid;
	int mode;
};

// shell information
struct info {
	char current_user[LINE_SIZE];
	char current_dir[PATH_SIZE];
	char pw_dir[PATH_SIZE];
	struct job *jobs[NUM_JOBS+1];
};

struct info *myShell;

// get the job id by pid of process
int get_job_by_pid(int pid) {
	int job_id = -1; // if not found
	struct process *process;
	int i;
	for (i = 0; i <= NUM_JOBS; i++) {
		if (myShell->jobs[i] != NULL) {
			for (process = myShell->jobs[i]->root; process != NULL; process = process -> next) {
				if (process->pid == pid) {
					job_id = i;
				}
			}
		}
	}
	return job_id;
}

// get the job by the job_id
struct job * get_job_by_id (int id) {
	if (id > NUM_JOBS) {
		return NULL;
	}
	return myShell->jobs[id];
}

int get_pgid_by_id (int id) {
	struct job * jobs = get_job_by_id(id);
	if (jobs == NULL) {
		return -1;
	}
	return jobs->pgid;
}

int count_num_process (int id, int filter) {
	int count = 0;
	struct process * process;
	if (id > NUM_JOBS || myShell->jobs[id] == NULL) {
		return -1;
	}
	for (process = myShell->jobs[id]->root; process != NULL; process = process->next) {
		if ((filter == PROC_FILTER_ALL) || (filter == PROC_FILTER_DONE && process->status == STATUS_DONE) 
			|| (filter == PROC_FILTER_REMAIN && process->status == STATUS_DONE)) {
			count++;
		}	
	}
	return count;
}

int get_next_job () {
	// if the job is null, then it is the next one
	int i;
	for (i = 1; i <= NUM_JOBS; i++) {
		if (myShell->jobs[i] == NULL) {
			return i;
		}
	}
	// if not exist then -1
	return -1;
}

int print_pids_of_job (int job_id) {
	if (job_id > NUM_JOBS || myShell->jobs[job_id] == NULL) {
		// job doesn't exist
		return -1;
	}
	struct process *process;
	for (process = myShell->jobs[job_id]->root; process != NULL; process = process->next) {
		printf("%d\n", process->pid);
	}
	return 0;
}

int print_job_status(int job_id) {
	if (job_id > NUM_JOBS || myShell->jobs[job_id] == NULL) {
		// job doesn't exist
		return -1;
	}
	struct process *process;
	printf("Job ID: %d\n", job_id);
	for (process = myShell->jobs[job_id]->root; process != NULL; process = process->next) {
		printf("PID: %d\n", process->pid);
		printf("Status: %s\n", STATUS_STRING[process->status]);
		printf("Command: %s\n", process->command);
	}
	if (process->next == NULL) {
		printf("|\n");
	} 
	else {
		printf("\n");
	}
	return 0;
}

// clear functions 
int release_job(int job_id) {
	if (job_id > NUM_JOBS || myShell->jobs[job_id] == NULL) {
		// job doesn't exist
		return -1;
	}
	struct job * job = myShell->jobs[job_id];
	struct process *process;
	struct process *temp;
	for (process = job->root; process != NULL;) {
		// free all contents of each process in the job
		temp = process->next;
		free(process->command);
		free(process->argv);
		free(process->in_path);
		free(process->out_path);
		free(process);
		process = temp;
	}
	// free the job
	free(job->command);
	free(job);
	return 0;
}

int insert_job(struct job * jobs) {
	// get the next free job on the list of jobs
	int job_id = get_next_job();
	if (job_id < 0) {
		return -1;
	}
	// put the job into the shell to execute
	jobs->id = job_id;
	myShell->jobs[job_id] = jobs;
	return job_id;
}

int remove_job(int job_id) {
	if (job_id > NUM_JOBS || myShell->jobs[job_id] == NULL) {
		// job doesn't exist
		return -1;
	}
	// free job, and check if it's null or not
	release_job(job_id);
	myShell->jobs[job_id] = NULL;
	return 0;
}

int check_completion(int job_id) {
	if (job_id > NUM_JOBS || myShell->jobs[job_id] == NULL) {
		// job doesn't exist
		return -1;
	}
	struct process * process;
	for (process = myShell->jobs[job_id]->root; process != NULL; process = process->next) {
		if (process->status != STATUS_DONE) {
			return 0;
		}
	}
	return 1;
}

int set_process_status (int pid, int status) {
	struct process *process;
	int success = -1;
	int i;
	for ( i = 1; i <= NUM_JOBS; i++) {
		if (myShell->jobs[i] == NULL) {
			continue; // jump to next execution of the loop without executing the rest of this loop
		}
		for (process = myShell->jobs[i]->root; process != NULL; process = process->next) {
			if (process->pid == pid) {
				process->status = status;
				success = 0;
			}
		}
	}
	return success;
}

int set_job_status (int job_id, int status) {
	if (job_id > NUM_JOBS || myShell->jobs[job_id] == NULL) {
		// job doesn't exist
		return -1;
	}
	struct process * process;
	for (process = myShell->jobs[job_id]->root; process != NULL; process = process->next) {
		if (process->status != STATUS_DONE) {
			process->status = status;
		}
	}
	return 0;
}

int wait_pid (int pid) {
	int status = 0;
	waitpid(pid, &status, WUNTRACED);
	if (WIFEXITED(status)) {
		set_process_status(pid, STATUS_DONE);
	}
	else if (WIFSIGNALED(status)) {
		set_process_status(pid, STATUS_TERMINATED);
	}
	else if (WSTOPSIG(status)) {
		set_process_status(pid, STATUS_SUSPENDED);
	}
	return status;
}

int wait_job (int job_id) {
	if (job_id > NUM_JOBS || myShell->jobs[job_id] == NULL) {
		// job doesn't exist
		return -1;
	}
	int num_process = count_num_process(job_id, PROC_FILTER_REMAIN);
	int wait_pid = -1;
	int wait_counter = 0;
	int status = 0;

	do {
		wait_pid = waitpid(myShell->jobs[job_id]->pgid, &status, WUNTRACED);
		wait_counter++;
		if (WIFEXITED(status)) {
			set_process_status(wait_pid, STATUS_DONE);
		}
		else if (WIFSIGNALED(status)) {
			set_process_status(wait_pid, STATUS_TERMINATED);
		}
		else if (WSTOPSIG(status)) {
			status = -1;
			set_process_status(wait_pid, STATUS_SUSPENDED);
			if (wait_counter == num_process) {
				print_job_status(job_id);
			}
		}
	} while (wait_counter < num_process);
	return status;
}

int command_character(char * command) {
	if (strcmp(command, "exit") == 0) {
		return COMMAND_EXIT;
	}
	else if (strcmp(command, "cd") == 0) {
		return COMMAND_CD;
	}
	else if (strcmp(command, "help") == 0) {
		return COMMAND_HELP;
	}
	else if (strcmp(command, "pwd") == 0) {
		return COMMAND_PWD;
	}
	else {
		return COMMAND_EXTERNAL;
	}
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

void update_cwd() {
	getcwd(myShell->current_dir, sizeof(myShell->current_dir));
}

int cd_func(int argc, char** argv) {
	if (argc == 1 || (argc == 2 && (strcmp(argv[1], "~") == 0))) {
		chdir(myShell->pw_dir);
		update_cwd();
		return 0;
	}
	if (chdir(argv[1]) == 0) {
		update_cwd();
		return 0;
	}
	else {
		printf("cd %s: Directory invalid, please try again\n", argv[1]);
		return 0;
	}
}

//get current directory
int pwd_func(char ** argv) {
//Using char *getenv(char *args) to search for the environment string
//Return -1: for error and return 1 for success command
  char * env;
  if((env = getenv("PWD")) == NULL)
  {
      fprintf(stderr,"%s: Fail pwd \n",argv[0]);
      return -1;
  }
  else
      printf("%s \n",env);
  return 1;

}

int exit_func() {
	printf("Exiting shell: Bye!\n");
	exit(0);
}

int help_func() {
  	printf("Annie's Mini-shell\n");
  	printf("The following are built in:\n");
	int i;
  	for (i = 0; i < NUM_BUILTINS; i++) {
    	printf("  %s\n", BUILTIN_STR[i]);
  	}
  	printf("Use the man command for information on other programs.\n");
  	return 1;
}

// check if there is any zombie process running 
void check_zombie() {
	int status;
	int pid;
	while ((pid = waitpid(-1, &status, WNOHANG|WUNTRACED|WCONTINUED)) > 0) {
		if (WIFEXITED(status)) {
			set_process_status(pid, STATUS_DONE);
		}
		else if (WIFSTOPPED(status)) {
			set_process_status(pid, STATUS_SUSPENDED);
		}
		else if (WIFCONTINUED(status)) {
			set_process_status(pid, STATUS_CONTINUED);
		}
		int job_id = get_job_by_pid(pid);
		if (job_id > 0 && check_completion(job_id)) {
			print_job_status(job_id);
			remove_job(job_id);
		}
	}
}

// Create a signal handler for Ctrl-C
void sigint_handler(int sig){
	write(1,"Terminating through signal handler\n",80);
	exit(0);
}

// execute builtin commands
int execute_command(struct process * process) {
	int status = 1;
	switch(process->type) {
		case COMMAND_EXIT:
			exit_func();
			break;
		case COMMAND_CD:
			cd_func(process->argc, process->argv);
			break;
		case COMMAND_HELP:
			help_func();
			break;
		case COMMAND_PWD:
			pwd_func(process->argv);
			break;
		default:
			status = 0;
			break;
	}
	return status;
}


int launch_process(struct job * job, struct process * process, int in, int out, int mode) {
	process->status = STATUS_RUNNING; // change status to running to signal launch
	// if not an external process and executed by the fn to execute builtin commands
	if (process->type != COMMAND_EXTERNAL && execute_command(process)) {
		return 0;
	}

	pid_t child;
	int status = 0;

	// create a child process
	child = fork();
	// fork failure
	if (child < 0) {
		return -1;
	}
	else if (child == 0) {
		// set up signals
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		//signal(SIGCHILD, SIG_DFL);

		process->pid = getpid();
		// identify the first process of the job
		if (job->pgid > 0) {
			setpgid(0, job->pgid);
		}
		else {
			job->pgid = process->pid;
			setpgid(0, job->pgid);
		}
		// set up for pipe
		if (in != 0) {
			dup2(in, 0); // set this for input
			close(in);
		}
		if (out != 1) {
			dup2(out, 1); // set this end for output
			close(out);
		}
		if (execvp(process->argv[0], process->argv) > 0) {
			printf("%s: Command not found\n", process->argv[0]);
			exit(0);
		}
		exit(0);
	}
	else {
		process->pid = child;
		if (job->pgid > 0) {
			setpgid(child,job->pgid);
		}
		else {
			job->pgid = process->pid;
			setpgid(child,job->pgid);
		}

		if (mode == FOREGROUND) {
			tcsetpgrp(0, job->pgid);
			status = wait_job(job->id);
			signal(SIGTTOU, SIG_IGN);
			tcsetpgrp(0, getpid());
			signal(SIGTTOU, SIG_DFL);
		}
	}
	return status;
}

// execution function of jobs in the loop for main
int run_job(struct job *job) {
	struct process * process;
	int status = 0;
	int in = 0;
	int pipeline[2];
	int job_id = -1;

	check_zombie();
	if (job->root->type == COMMAND_EXTERNAL) {
		job_id = insert_job(job);
	}

	for (process = job->root; process != NULL; process = process->next) {
		if (process == job->root && process->in_path != NULL) {
			in = open(process->in_path, O_RDONLY);
			if (in < 0) { // if can't open
				printf("error: no such directory %s\n", process->in_path);
				remove_job(job_id);
				return -1;
			}
			if (process->next != NULL) {
				pipe(pipeline);
				status = launch_process(job, process, in, pipeline[1], PIPELINE);
				close(pipeline[1]);
				in = pipeline[0];
			}
			else {
				int out = 1;
				if (process->out_path != NULL) {
					out = open(process->out_path, O_CREAT | O_WRONLY | S_IRUSR | S_IWUSR | S_IRGRP| S_IROTH);
					if (out < 0) {
						out = 1;
					}
				}
				status = launch_process(job, process, in, out, job->mode);
			}
		}

		if (job->root->type == COMMAND_EXTERNAL) {
			if (status >= 0 && job->mode == FOREGROUND) {
				remove_job(job_id);
			}
			else if (job->mode == BACKGROUND) {
				print_pids_of_job(job_id);
			}
		}
	}
	return status;
}

// parse process command -> make a process
struct process * parse_process_command(char * process_command) {
	int size = TOKEN_SIZE;
	int posn = 0;
	// copy so that we don't lose the pointer to the string when parsing
	char * command = strdup(process_command);
	char * token;
	char ** tokens = (char**)malloc(size * sizeof(char*));

	if (! tokens) {
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}
	
	// parse the command - do not need to consider wildcards
	token = strtok(process_command, DELIMITERS);
	while (token != NULL) {
		tokens[posn] = token;
		posn++;
		token = strtok(NULL, DELIMITERS);
	}

	// redirection
	int i = 0;
	int argc = 0;
	char * in_path = NULL;
	char * out_path = NULL;

	while (i < posn) {
		if (tokens[i][0] == '<' || tokens[i][0] == '>') {
			break;
		}
		i++;
	}
	argc = i;

	for (; i< posn; i++) {
		if (tokens[i][0] == '<') {
			if (strlen(tokens[i]) == 1) {
				in_path = (char *)malloc((strlen(tokens[i+1]) + 1) *  sizeof(char));
				strcpy(in_path, tokens[i+1]);
				i++;
			}
			else {
				in_path = (char *)malloc(strlen(tokens[i]) *  sizeof(char));
				strcpy(in_path, tokens[i]+1);
			}
		}
		else if (tokens[i][0] == '>') {
			if (strlen(tokens[i]) == 1) {
				out_path = (char*)malloc((strlen(tokens[i+1]) + 1) *  sizeof(char));
				strcpy(out_path, tokens[i+1]);
				i++;
			}
			else {
				out_path = (char *)malloc(strlen(tokens[i]) *  sizeof(char));
				strcpy(out_path, tokens[i]+1);
			}
		}
		else {
			break;
		}
	}

	for (i = argc; i <= posn; i++) {
		tokens[i] = NULL;
	}

	struct process * new_proc = (struct process *)malloc(sizeof(struct process));
	new_proc->command = command;
	new_proc->argc = argc;
	new_proc->argv = tokens;
	new_proc->in_path = in_path;
	new_proc->out_path = out_path;
	new_proc->pid = -1;
	new_proc->type = command_character(tokens[0]);
	new_proc->next = NULL;

	return new_proc;
}

struct job * parse_line(char * line) {
	line = string_trim(line);
	char * process_command = strdup(line);

	struct process * root = NULL;
	struct process * process = NULL;
	char * temp = line;
	char * character = line;
	char * segment;
	int segment_length = 0;
	int mode = FOREGROUND;

	// check to see if job is declared as bckground run
	if (line[strlen(line) - 1] == '&') {
		mode = BACKGROUND;
		line[strlen(line) - 1] = '\0';
	}

	while (1) {
		if (*character == '\0' || *character == '|') {
			segment = (char*)malloc((segment_length + 1) * sizeof(char));
			strncpy(segment, temp, segment_length);
			segment[segment_length] = '\0';

			struct process * new_proc = parse_process_command(segment);
			if (!root) {
				root = new_proc;
				process = root;
			}
			else {
				process->next = new_proc;
				process = new_proc;
			}

			if (*character != '\0') {
				temp = character;
				while (*(++temp) == ' ');
				character = temp;
				segment_length = 0;
				continue;
			}
			else {
				break;
			}
		}
		else {
			segment_length++;
			character++;
		}
	}

	struct job * new_job = (struct job*)malloc(sizeof(struct job));
	new_job->root = root;
	new_job->command = process_command;
	new_job->pgid = -1;
	new_job->mode = mode;
	return new_job;
}

char* read_line() {
	int size = LINE_SIZE;
	int posn = 0;
	char * buffer = malloc(sizeof(char) * size);
	int num_char;

	if (!buffer) {
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1) {
		num_char = getchar();

		if (num_char == EOF || num_char == '\n') {
			buffer[posn] = '\0';
			return buffer;
		}
		else {
			buffer[posn] = num_char;
		}
		posn++;
		if (posn > size) {
			size += LINE_SIZE;
			buffer = realloc(buffer, size);
			if (!buffer) {
				fprintf(stderr, "Allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}


void shell_loop() {
	char * line;
	struct job * job;
	int status = 1;


	do {
		printf("mini-shell>");
		line = read_line();
		if (strlen(line) == 0) {
			check_zombie();
			continue;
		}
		job = parse_line(line);
		status = run_job(job);
	} while (1);
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

	pid_t pid = getpid();
	setpgid(pid, pid);
	tcsetpgrp(0, pid);

	myShell = (struct info *)malloc(sizeof(struct info));
	getlogin_r(myShell->current_user, sizeof(myShell->current_user));
	struct passwd *pw = getpwuid(getuid());
	strcpy(myShell->pw_dir, pw->pw_dir);
	int i;
	for (i = 0; i < NUM_JOBS; i++) {
		myShell->jobs[i] = NULL;
	}
	update_cwd();
}

int main(int argc, char ** argv) {
	// get the signal initialization here: quit when control-C
	printf("You can terminate by pressing Ctrl+C\n");

	shell_init();

	// Run command loop
	shell_loop();

	// perform any shutdown/clean-up

	return EXIT_SUCCESS;
}


