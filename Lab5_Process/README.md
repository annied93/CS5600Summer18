# Process API

In this lab, we are going to explore the process API to get familiar with launching processes from within our program.

# Part 1 - fork

Remember that when we use fork(), we are creating a child processes that is identical to the parent process. The child process gets a copy of everything that the parent had, but the memory is distinct, such that modificiations in either the parent or child do not effect each other.

Let us go ahead and try a simple example with fork.

## example1.c

The following program is our version of a "hello world" to the fork command.

```
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

	pid_t pid;
	int x = 1;

	pid = fork();

	if(pid==0){
		printf("child: x=%d\n",++x);
		return 0;	
	}

	// Parent
	printf("parent: x=%d\n", --x);

	return 0;
}

```

* Save, compile, and run the following program: `gcc example1.c -o example1`

Let's try another example this time:

## example2.c

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

	pid_t pid;
	int x = 1;

	pid = fork();

	if(pid==0){
		printf("Terminating Child, PID= x=%d\n",getpid());
		exit(0);	
	}else{
    	    // Run the parrent
	    printf("Running parent forever(pretend I am a server): pid=%d\n",getpid());
    	    while(1){
    	    }
  	}

	return 0;
}
```

* Save, compile, and run the following program: `gcc example2.c -o example2`

What happens when you compile and run this code? (Run with `./example2 &` the ampersand returns control back to shell.

You can type `ps` into the terminal. You will notice that one of the processes is labeled <defunct>, and that id will actually match our childs pid. What happened here, is that we have not reclaimed (or reaped) our children. Our child is said to be a zombie process, in that it is occupying and using resources, but not doing any useful work.
  
Let's get rid of our processes now. Type `kill somenumber` where somenumber is the pid of your parent and child process (Note that killing the parent should kill the child process as well).
  
Now what if we flip the scenario where our child runs forever?

## example3.c

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

	pid_t pid;
	int x = 1;

	pid = fork();

    if(pid==0){
    	printf("Running child forever now pid=%d\n",getpid());
    	while(1){
    	}
    }else{
	// Run the parrent
	printf("Terminating parent pid=%d\n",getpid());
	exit(0);
    }

	return 0;
}
```

* Save, compile, and run the following program: `gcc example3.c -o example3`

We will have the same problem, and we need to run the `ps` command again(to see which processes are running), and then the `kill #` command.

## Synchronization

Now this seems like a bit of pain, so how can we fix this problem?

The answer is to use a synchronization function called `wait`. `int wait(int *child_status)` will reap a child for us. In this case, a parent will wait to execute until a child process has finished.

## example4.c

```
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
```

* Save, compile, and run the following program: `gcc example4.c -o example4`

## execve

In order to execute other programs (i.e. a more specific process), we have the execve command.

It looks like the following:

`int execve(char* filename, char* argv[], char*envp[])`

- filename: This is the script or file we are going to execute
- argv: This is our argument list (which we are familiar with). argv[0] = filename.
- envp: These are any specific environment variables we want to set.
  - More information can be found here: [https://en.wikipedia.org/wiki/Environment_variable]

Let's try a simple example with 'ls'

## example5.c

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

	char* myargv[16];
	myargv[0]="/bin/ls";
	myargv[1]="-F";
	myargv[2]=NULL; // Terminate the argument list
	
	if(fork()==0){
		// Executes command from child then terminates our process
		execve(myargv[0],myargv,NULL);
		printf("Child: Should never get here\n");
		exit(1);
	}

	printf("This always prints last\n");
	
	return 0;
}
```

* Save, compile, and run the following program: `gcc example5.c -o example5`

Go ahead and modify the above code and try to run some other processes that may be of interest.

# Part 2 - Terminal Programs

To get some practice, run the following commands (skim the man pages) to understand what information you can retrieve from the OS:

- proc/
- top
- ps
- kill



# Lab Deliverable

- Commit the source code for the 5 examples (example1.c example2.c exmample3.c example4.c example5.c).
- Write a small program using fork and/or execve to run a series of at least 3 other programs.
  - You are welcome to be as creative as you like!

# More resources to help

- https://www.networkworld.com/article/2693548/unix-viewing-your-processes-through-the-eyes-of-proc.html

# Going Further

- Investigate the different command shells: https://en.wikipedia.org/wiki/Comparison_of_command_shells
