# Lab 8 Multiprocessing - Artists!

Michaelangelo, Picasso, Monet, Kahlo, Cassatt, all legendary artists of their time. For this in-class lab, you are going to be painting a masterpiece, or rather--help a bunch of competitive artists work on a masterpiece together.

The trouble is though, these legendary artists do not like other artists to be in their workspace (We will refer to artists as 'processes' and vice versa). The artists all have been contracted to work on the same canvas, although as they paint they will wander off to their own section of the canvas to create a painting. In order to emulate this, we are going to build a multi-processing system. That is, using multiple processes and threads to build a final masterpiece.


## Part 1 - Processes

Our artists prefer to paint in Unix. Unix faciliates parallel processing natively in our system by maintaining multiple processes that are viewed independently.

### Task 1

Let us see how parallel we can get (i.e. how many artists) by analyzing our machines. 

- Run:  `cat /proc/cpuinfo | grep "cpu cores"` 
  - The above command tells us how many cores our systems have. 
  - (The last time I checked there are 8 cpu cores on each of our machines. It may be convenient for us to work in multiples of 8 then, which we can always increase later).

### Task 2

Let us now think about how we can spawn multiple processes from the same program.

- (Some starter code is below)
- Write a program that spawns 8 child processes using fork() and prints out (from the parent) all of the child process ids(pids).
- For each of the child processes, they should call a function called `paint(int workid)` 
   - The paint function takes in one argument the prints out the painter's number (This should be some other unique identifier passed on from the painter).

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// Add your paint function here

int main(int argc, char** argv){

        int numberOfArtists = ???;

        for(int i =0; i < numberOfArtists; i++){
                // (1) Perform a fork
                
                // (2) Make only the child do some work
                
                // (3) Print out the child pid (from the parent--mostly for logging purposes)
        }

        printf("parent is exiting(last artist out!)\n");

return 0;
}
```

### Task 3

Something that can be tricky with launching multiple processes, is understanding that we get an exact copy of the process (code, heap, stack, etc.) 

Let us modify our Task 2:
- I want you to malloc an integer array with 8 integers at the top of *main* function. 
- Remember to free the memory at the end of our  *main* function.
- Run the program to see that it runs properly.
- Second, now run the program in valgrind: `valgrind ./whatever_your_program_name_is
  - Is there any problem?
  - Assuming you made the modifications specified, there should be!
  - Memory leak! Where are we forgetting to reclaim our memory?

### Task 4

Have you been noticing that anything weird happens if you run your program enough times? It is possible that not all of your proceses are running, and they may not be running quite in order.

Let us fix that--I have two possible solutions to offer.

- The first is that we need to make use of the `waitpid` command. This will wait our parent on a pid. 
  - But which pid is it? One way to keep track would be to have an array in the parent that keeps track of all of the id's and waits on those pids.
- The second is a little trick you can put in your parent. `while((wpid = wait(&status)) > 0);` wpid is a pid_t and status is an int.   
  - What this does is essentially the same as above, making sure every child processe terminates.

### Task 5

Now we have talked about forking processes, which is one way to achieve parallelism. However, it becomes quite difficult to really do work on shared tasks. Remember, our artists have been contracted to work on the same canvas.

Alright, let us now allow our artists to work on one giant masterpiece.

- Create global 2D array called colors `char colors[64][64*3].` 
  - I have choosen 64x64 for the dimensions, and we will need to store 3 colors
  - The first dimension of the array you can think of as the 'row' an artist is painting on.
- Next, read the man page for vfork(`man vfork`)
- Discuss with your partner what is different about vfork different than fork()?

Scroll down a bit to see the answers after reading the man page.

```
VFORK(2)                                        Linux Programmer's Manual                                        VFORK(2)

NAME
       vfork - create a child process and block parent

SYNOPSIS
       #include <sys/types.h>
       #include <unistd.h>

DESCRIPTION
   Standard description
       (From  POSIX.1)  The vfork() function has the same effect as fork(2), except that the behavior is undefined if the
       process created by vfork() either modifies any data other than a variable of type pid_t used to store  the  return
       value  from  vfork(), or returns from the function in which vfork() was called, or calls any other function before
       successfully calling _exit(2) or one of the exec(3) family of functions.

   Linux description
       vfork(), just like fork(2), creates a child process of the calling process.  For  details  and  return  value  and
       errors, see fork(2).

       vfork()  is a special case of clone(2).  It is used to create new processes without copying the page tables of the
       parent process.  It may be useful in performance-sensitive applications where a child is created which then  imme‐
       diately issues an execve(2).

       vfork()  differs from fork(2) in that the calling thread is suspended until the child terminates (either normally,
       by calling _exit(2), or abnormally, after delivery of a fatal signal), or it makes a  call  to  execve(2).   Until
       that  point, the child shares all memory with its parent, including the stack.  The child must not return from the
       current function or call exit(3), but may call _exit(2).

       As with fork(2), the child process created  by  vfork()  inherits  copies  of  various  of  the  caller's  process
       attributes  (e.g., file descriptors, signal dispositions, and current working directory); the vfork() call differs
       only in the treatment of the virtual address space, as described above.

       Signals sent to the parent arrive after the child releases the parent's memory (i.e., after the  child  terminates
       or calls execve(2)).
```       


- Firstly, memory is shared! (Now we can access the *colors* array from every child)
  - (We can also now get rid of any additional frees we had in our child process since memory is shared)
- Secondly, your child processes should not terminate using return 0; that will throw an error message. exit(0) however is fine.
- We tend to think of vfork() as a 'lightweight process'. Because there is no copying of data from one process to another, vfork is fast! (You can imagine us consuming quite a bit of memory by copying colors over and over again)
- Finally, we do not need to 'wait' By default the parent will execute after the child from vfork has terminated.

### Task 6

- Update your program with vfork
- Have vfork launch 64 processes 
  - (Note that you can launch more if you like, and increase the color array--but your machine may have a soft limit of 100 or so processes).
  - Run `ulimit -u` to see the limit of processes you have. Mine is 175 for the CCIS machines (yours may be greater or less).
- Within the paint function, have each process work on one row of our color array. 
  - That is, your artist should write one value to each of the 64x3 values in the color array for its particular row (remember, artists do not share rows).
- At the end of your program, write out a file called 'vfork.ppm' that contains the contents of your masterpiece(fprintf will be useful for writing out intgers, and fputs for any strings). 
  - As a reminder, you can write to a file using the following tutorial: https://www.tutorialspoint.com/cprogramming/c_file_io.htm.
  - The format you are writing to is the PPM format here: http://netpbm.sourceforge.net/doc/ppm.html 
    - A sample for writing out a .ppm image is shown below.
    - (There should be a magic number, dimensions, and  maximum range)
    
```
// Write out the PPM file
	// If you are looking at this solution, it could
	// be better laid out in a 'save function'
	FILE *fp;
	fp = fopen("masterpiece.ppm","w+");
	fputs("P3\n",fp);
	fputs("64 64\n",fp);
	fputs("255\n",fp);
	for(int i =0; i < 64;i++){
		for(int j =0; j < 64*3; j++){
			fprintf(fp,"%d",colors[i][j]);
			fputs(" ",fp);		
		}
		fputs("\n",fp);
	}
	fclose(fp);
```

### Task 7

Okay, you have done the work, now times for some foundational reading.

- First up, the man pages. `man shm_overview` (~2 minute read)
- Next up is reading on how to see processes running `man ps`
- Why don't we just search all of the man pages. Type `man -K shared` and see what comes up. 
  - 'q' quits the man page, Use Ctrl+D to skip pages, Ctrl-C to quit, and /'whatever' to search within man pages. There will likely be many pages!

## Part 2 - Threads

Okay, so we have refreshed ourselves on how processes work and extended our knowledge to using vfork and shared memory. Now we are going to use [posix threads](https://computing.llnl.gov/tutorials/pthreads/) which are yet another technique for multiprocessing.

Previously when we looked at using multiple processes with our artists problem, we were able to get away with what is known as data parallelism because each process worked on a specific row in the final image. We will repeat the same task with threads, and so long as they work in mutually exclusive sections of the canvas, a similar solution will work!

### Task 1 - Artists
#### Prompt/Specification

Michaelangelo, Rapheal, Donatello, and Leonardo are amongst great artists who have been commissed to paint on a shared 64x64 canvas that starts out white(RGB of (255,255,255) on each pixel).

- This time, launch 64 threads (Do not use vfork).
- Have each artists `paint` in parallel to `colors`.
  - Think about if you need to use locks or not.

## Lab Deliverable

1. Generate an image using vfork that uses multiple processes. You can save the image as vfork.ppm.
2. Generate an image using pthreads using multiple threads and locks. You can save the image as threads.ppm.

## More Resources to Help

* pthreads tutorial: https://computing.llnl.gov/tutorials/pthreads/

## Going Further

- A nice book on multiprocessing https://www.amazon.com/Art-Multiprocessor-Programming-Revised-Reprint/dp/0123973376
- A few commands on multiprocessing summarized that you should be familiar with: http://faculty.salina.k-state.edu/tim/CMST302/study_guide/topic8/multi.html



