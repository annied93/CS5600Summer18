# Due Date: June 1, 2018 at 11:59 PM.

TODO Please edit the following information in your assignment

- Name: An Dinh
- How many hours did it take you to complete this assignment? 20-25 hours (most of it was spent on reading about Assembly)
- Did you collaborate with any other students/TAs/Professors? I have help from the professor and other students through Piazza posts.
(Really appreciate the help you guys have given me!)
- Did you use any external resources? (Cite them below)
  - https://www.cs.cmu.edu/~fp/courses/15213-s06/misc/asm64-handout.pdf
  - http://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html#memory
  - Youtube video: https://www.youtube.com/watch?v=5iQkR69H_1M
  - Stack Overflow posts about assembly syntax (too many to list here)
- (Optional) What was your favorite part of the assignment? To be honest, I think this assignment is very very difficult, especially part 
2 when we have to write our own assembly code. My code is a lot borrowed from Part 1 for the print, the compiled .s file, and the advice
of other people. I am still not very sure about assembly but hopefully I did not do terribly on the bubblesort case. 
- (Optional) How would you improve the assignment? A comprehensive guide on how to do assembly from the beginning would be nice?


# Introduction

For this assignent you are going to be jumping into Assembly programming! Jump in and get started!

**Note** It is important that you work on the assignment on the ccis servers (That is where you are testing, and your machine may have a different architecture!). I recommend using a text editor like vim, nano, or emacs while working remotely. Otherwise, think about how you might streamline your workflow.

# Part 1 Assembly - A First Program

The first task is to actually just write a 'hello world' in assembly.

- Enter the following text into a file called *hello.s*
- ```
	  # Example for x86-64 processor using AT&T Style syntax
	.global _start

	.text

	_start:
		# Write a message to the screen
		mov $1, %rax
		mov $1, %rdi
		mov $message, %rsi
		mov $13, %rdx
		syscall

		# Exit the program
		mov $60, %rax
		xor %rdi, %rdi
		syscall

	message:
		.ascii "Hello, World\n"
  ```
- Save the file and then we will assemble the program using the GNU Assembler (https://en.wikipedia.org/wiki/GNU_Assembler)
  - `gcc -c hello.s`
  - This step builds an object file.
  - We now need to create a binary from this file using a tool called So *ld*
  - `ld hello.o -o hello`
    - Run `man ld` to learn more.
  - When this has been completed, push hello.s to your repo, and move on!

There are a few interesting things with this program.
1. First, there is a *global* symbol. The global sets up the starting point, as our program needs some entry point. You could think of '_start' as 'main' like in a C program. You can learn more here: http://web.mit.edu/gnu/doc/html/as_7.html#SEC89
2. The next directive (remember, lines that start with a '.' are directives) is .text. These are where our instructions start. https://stackoverflow.com/questions/14544068/what-are-data-and-text-in-x86
3. The next few lines are moving some values into registers. The first statement moves the immediate value $1 into register %rax. A few more lines down we see a syscall.
  1. So first off, what is a syscall? In short, it is a call to a function built into the operating system (More here: https://www.geeksforgeeks.org/operating-system-introduction-system-call/).
  2. To figure out which syscall it is, use this resource: https://filippo.io/linux-syscall-table/
4. Finally at the end there is a label ('message:') with a string literal (.ascii "Hello, World\n"). https://docs.oracle.com/cd/E26502_01/html/E28388/eoiyg.html


# Part 2 - Assembly Sort

NOTE: My file name for this part is bubblesort.s -> if you could check that for this part it would be great!

Okay, now it is time to implement some assembly of our own! You are going to write some x86-64 with AT&T style syntax.

The program you are going to write is a sort routine to sort some numerical data (You will sort the same 9 integers I have provided in the C function). As a template, this is the 'C Program' you will emulate. Save your program as *sort.s*

It may be helpful to first implement the C program before jumping into assembly. Note, you can add any helper routines you like in your assembly. Expect to write in the range of 50-60 lines of assembly depending on your implementation.

```
// Do not add any more 'includes'
#include <unistd.h>

// Implement your sort function here
// This can be any sort function you like(brute force, bubble, quick, etc.)
void sort(int* array, int size){
}

int main(){

        // Initialize the data array
        // Do not modify this data.
        // You will see a nice printout if your data is sorted correctly with these values :)
        int data[9] = {116,67,69,102,101,102,111,114,65};

        // Call sort
        sort(data,9);
        // Iterate through the data
        int i =0;
        // Use the 'write' system call. 
        // Do NOT use printf anywhere in your assembly.
        for(i = 0 ; i < 9; ++i){
                write(1,&data[i],sizeof(data[i]));
        }

        return 0;
}

```

Note, it may be beneficial to look at this ascii table to see how numbers and letters are represented. https://www.asciitable.com


# Part 3 - GDB Introduction 

GDB is a debugger for the programs that we are compiling with GCC. It helps us search and step through programs and track down errors in our program.

Watch the following video from Greg Law: *Give me 15 minutes and I'll change your view of GDB*
https://www.youtube.com/watch?v=PorfLSr3DDI

Then follow the walkthrough below based on the video to get some practice.

## GDB General Tasks

Browse the following tutorial to get familiar with GDB: https://www.cs.cmu.edu/~gilpin/tutorial/

## A sample program

Enter the following program below and save it somewhere (It is the same one from the Greg Law video).
```
#include <stdio.h>

int main(void){
        int i =0;
	printf("Hello, world\n");
        printf("i is %d\n",i);
	i++;
	printf("i is now %d\n",i);

        return 0;
};
```

Compile the program with debug symbols
`gcc hello.c -g -o hello`

`gdb ./hello`

## GDB TUI (Text User Interface)

Here are the instructions for the TUI part of the video roughly written out. Practice using them with the program above.

- Type: `start`
- Type: `list`
- You can type 'next' to move line by line.
- Then press [Ctrl+x a] (Press Ctrl and x at the same time, then press 'a' key after)
  - (Pressing [Ctrl+x a] again will bring you back to just the gdb terminal)
- Typing in 'next' again moves another line (or simply hit 'enter' which repeats the previous command).
- Ctrl+L will clear the screen after the text prints to clean it up.
- Now try hitting [Ctrl+x 2]
  - You will now see multiple windows with the assembly and the source code.
  - Pressing [Ctrl+x o] will cycle between the windows.
  - Hitting [Ctrl+x 2] a couple of times will cycle through the windows to show what is in the registers.
    - Typing `tui reg` will give some options to see specific registers.
      - For example, `tui reg float` shows the floating registers.
      - For example, `tui reg general` displays the general purpose registers.
 - Hit [Ctrl+x 1] to go back to the first view and see your source code.
   - Typing in some commands like 'layout src' will bring you back to your source view.
     - Typing in 'layout asm' will show you the assembly.
- More: https://sourceware.org/gdb/onlinedocs/gdb/TUI-Commands.html#TUI-Commands
- More: https://sourceware.org/gdb/onlinedocs/gdb/TUI-Keys.html#TUI-Keys
- More resources on GDB TUI: https://sourceware.org/gdb/onlinedocs/gdb/TUI.html
  
## GDB Python	

Here are the Python commands written out as well.

Python is a general purpose scripting langauge--perhaps you have even used it!

What is neat about Python is that the languages interpreter can be embedded into other tools(and even other programming languages!).
	
- Try the following within GDB:
  - Type: `python print("hello gdb")`
  - Type: `python import os`
  - Type: `python print("my pid is %d" % os.getpid()) # pid = process id`
  - If you have inserted any breakpoints:
    - (Create one with `b main`)
    - Type: `python print(gdb.breakpoints())`
    - Type: `python print(gdb.breakpoints()[0].location)`
    - With Python, you can even create breakpoints!
      - `python gdb.Breakpoint('7')`
    
- Finally typing in `quit` will exit gdb when you are done.

## Small exercise

Run gdb on the program *prog1*

It has been compiled with debugging symbols so you may investigate it. Answer the following questions below.

## **TODO (Add your answers here)**

1. What was the bug? 
- The program never allocate memory space for the head node pointer before trying to assign value to it
2. What line did the bug occur?
- Line 14 (no malloc) and 15(assigning value to head->data)
3. What is the fix?
- At line 14, we should have: node_t* head = (node_t*)malloc(sizeof(node_t));
- This will properly allocate memory to the head node for us to be able to use it. 
	

## (Bonus Part) Aside: Core dump

In the Greg Law video, he begins to investigate a Core dump file. What exactly is a coredump?
- A coredump is a file containing the memory address of the process that is generated when the process is interrupted and terminated by the OS unexpectedly, due to some fault in the program.
- The most common reason for core dump is due to pointer error. Given the core dump might be sporadic dump (like the one in part 3 of this assignment), this is due to uninitialized pointer. 

Read here: https://wiki.archlinux.org/index.php/Core_dump

# Resources to help

- A full 90+ minutes of GDB debugging: https://www.youtube.com/watch?v=713ay4bZUrw

# Rubric

- 25% Part 1 - Does your hello.s work.
- 55% Part 2 - Does your sort.s work.
  - Avoid using any C library functions--only system calls!
- 20% Part 3 - GDB
  - Did you answer everything correctly?

# Feedback Loop

(An optional task that will reinforce your learning throughout the semester)

- Continue using the Compiler Explorer as you write your C code. https://godbolt.org

