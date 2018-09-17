# Linker - "Respect the Linker"

In this lab, we are going to explore a linker trick with our knowledge of how linkers work.

# Part 1 - Making a library

Now that we know a little about how the linker works, lets make some shared object libraries.

Your first step is compile and run the lottery.c program. lottery.c is a program that generates the four random numbers used in the mega-millions lottery. It is in fact, a top secret program! This program is so top secret, we would not want to let anyone know about how the random numbers are generated.

Take a moment to look at the lottery.c file now to understand what it is doing.

Next compile and run the program lottery to generate the mega-millions number.
`gcc lottery.c -o lottery`

Uh oh, *you should be getting some linker error messages like this* -- We forgot to link in our library!

```
/tmp/lottery-2362b1.o: In function main:
lottery.c:(.text+0x58): undefined reference to TopSecretGenerateRandomNumber
lottery.c:(.text+0x87): undefined reference to TopSecretGenerateRandomNumber
lottery.c:(.text+0xb6): undefined reference to TopSecretGenerateRandomNumber
lottery.c:(.text+0xe5): undefined reference to TopSecretGenerateRandomNumber
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```

Let us investigate more closely the lottery.c file to see what is going on. Look for the function TopSecretGenerateRandomNumber function near the top of the source code.

```
// This function is external to our
// code, but will be found by the linker
extern int TopSecretGenerateRandomNumbe();
```

The `extern` means that the function is located externally to our current source code file.

Let's next generate a .o file with `gcc -c lottery.c`, and we will then work on confirming that external function is undefined.

## Objdump

Run `objdump -t lottery.o` on the lottery.o file that was generated. Can you confirm TopSecretGenerateRandomNumber is not defined? 

```
0000000000000000         *UND*	0000000000000000 TopSecretGenerateRandomNumber
0000000000000000         *UND*	0000000000000000 fflush
0000000000000000         *UND*	0000000000000000 printf
0000000000000000         *UND*	0000000000000000 srand
0000000000000000         *UND*	0000000000000000 stdout
0000000000000000         *UND*	0000000000000000 time
```

Remember that the `-t` symbol is outputting the symbol table. That is, a list of functions that are found in the object file. The meanings of each of the columns can be uncovered by running `man objdump` and then scrolling to see what **-t** means.

## Modularity

Okay, so perhaps some savvy programmers made the function modular. They hid the TopSecretGenerateRandomNumber function from us so we could not see in our source code. And there are many lotteries after all, so they probably made it a shared library for others to borrow. If I investigate the contents of this folder, I find a file called libsecret.so (.so extension for shared object file). 

There is no source code however, so we'll do the next best thing. Run `objdump -t libsecret.so` to investigate. What are some of the functions you see if you scroll around?

```
0000000000000000 g     F .text	0000000000000015 TopSecretGenerateRandomNumber
0000000000000000         *UND*	0000000000000000 rand
```

(The *g* means that the symbol is external, and the F .text means there is a function found in the .text section of the object file)
The libsecret.so file was compiled with: `gcc -fPIC -shared secret.c -o libsecret.so` to create the file. Unfortunately, there is no secret.c file to be found (Well, there is in the 'hidden' folder if you really want to, but pretend it does not exist).

But the good news is there is a function called TopSecretGenerateRandomNumber available in libsecret.so. Perhaps we can link this in!

Another tool called *nm*  shows us the symbols within an executable as well (there is another tool called llvm-nm as well)

Try running the command now to see. `nm -g libsecret.so`

## Linking two .o files

Okay, let us now try `gcc lottery.c libsecret.so -o lotto`

Ah ha, no errors!

Let us try to run `./lotto`

`./lotto: error while loading shared libraries: libsecret.so: cannot open shared object file: No such file or directory`

Hmm another error, but we can see libsecret.so within our current directory.

There is a nice tool that can help us debug linker errors called ldd. ldd prints out shared library dependencies. Investigate with `man ldd` to se the general description.

```
LDD(1)                                        Linux Programmer's Manual                                        LDD(1)

NAME
       ldd - print shared library dependencies
```

Now run: `ldd ./lotto`
```
	linux-vdso.so.1 =>  (0x00007ffeec3df000)
	libsecret.so => not found
	libc.so.6 => /lib64/libc.so.6 (0x00007ffb2b93c000)
	/lib64/ld-linux-x86-64.so.2 (0x00007ffb2bd18000)
```

We see that libsecret.so is not found (on the second row). Okay, so let us add our library to a path where it can be found (it is in the same folder afterall though...) The truth is that our linker searches in a few directories for shared libraries. One of the directories that it looks at, is set by an environment variable from the system. In this case, it is called LD_LIBRARY_PATH.

Now where we want to look, is in the current directory.

So if we run: `LD_LIBRARY_PATH=. ./lotto`

Then tada! Finally we see our output. What the command does is loads any libraries in the current path(the single dot at the start is the path.) for the current program. Run this line a few times when you are confident it will work.

## I never win the lottery

Now the lottery folks are protecting this `./lotto` program that I have shared with you. It is very secret. Unfortunately, I have never won the lottery and today I would like to win. With what we have learned about linkers, we can hack the lottery!

Write a program called hack.c that is the following.

```
#include <stdlib.h>

// Our version of the random number generator
int TopSecretGenerateRandomNumber(){
	return 42;
}
```

Compile it as a shared library: `gcc -shared -fPIC hack.c -o libhack.so`
(Note: the -fPIC creates position independent code, and -shared makes this a shareable library as opposed to static)

The order in which the linker searches for shared libraries starts by the previous command we used (i.e. LD_LIBRARY_PATH). However, we can point the linker(i.e. force the linker to follow our defined behavior) the linker to load some libraries first. Then when the linker searchers, it will be satisfied once it finds all of the libraries it needs when resolving undefined references.

## LD_PRELOAD

Let us preload a new library, our 'hack' library that is going to help us win the lottery. What we are doing is exporting another environment. Note that with export, this preserves a change in our environment.

`export LD_PRELOAD=$PWD/libhack.so lotto`

Now let us build this source code into a library, and we will compile it with clang.

Now run again once again with: `LD_LIBRARY_PATH=. ./lotto`
```
	And the winning numbers are...
	42
	42
	42
	42
	42 42 42 42...who would have guessed!
```
**Wait a second as the audience gasps...it looks like something is wrong with the lottery!**


So as we are noticing, the linker is actually quite powerful! But this also teaches us one of the weaknesses of shared libraries, because they are allowed to be preloaded before runtime. This trick can often be quite handy for debugging purposes as well. We can test out different functions for example when debugging or print additional information.

We could for example write our own memory allocator by overriding the malloc and free functions. In fact, we could override nearly any well known C-library function by linking in our own implementation dynamically. or perhaps we can use some compiler hooks.(

## Final Step

1. Run `echo $LD_PRELOAD` to see the environment variables you have set. We will want to clean up our environment back to where we started.
2. Run `unset LD_PRELOAD` to get rid of any exported variables and return to our default environment (You can test this works by doing `echo $LD_PRELOAD` to ensure this is empty).

## Check your understanding

1. Understand we can save compile time by only compiling the libraries we change or the files we have source code for.
2. Understand how the linker helps find the code needed for our executable. The code dos not live altogether in one executable.
3. (Some example is provided below) Try compiling a small program with g++ and the -g option. That is, using the c++ programming language instead of C (g++ is the GNU C++ compiler). What do you notice that is different about the functions(*Hint* search for foo)? Do we have some information that could be useful for LD_PRELOAD?

```
// compile with: g++ -g -c hello.cpp
// Investigate with:  objdump -t hello.o 
int foo(int a, int b){
	return a+b;
}

int main(){

	foo(1,2);	

	return 0;
}
```

## Part 2 - Compiler Hooks

With the linker, we have essentially hooked into existing functions that we know about, or could otherwise decipher if we have additional debugging information (**Another word of caution** never distribute commercial software compiled with the debuggins symbols.)

However, we have learned that compilers at some point have to touch every part of a code base. They are after all, responsible for compiling all of the code! This can make them quite powerful tools, especially if we want to instrument programs and uncover more information.

The GCC Compiler in particular has some neat compiler hooks for instrumentation. That is, a *hook* is a way for us to add in our own functions to help us probe a programs behavior. This can be very useful for learning what a program is doing.

Navigate to the `trace/` folder with `cd trace/`

Within it, you will see two files main.c and trace.c. Both are quite small, investigate them now.

What you will see in the trace.c is some oddly named functions--two to be exact. Each of these functions has an attribute marked on them to not be instrumented. Why that is the case? Well, these functions are going to be called at the start and end of every other function that is called in a program. That is, our compiler will automatically inject these functions into every other function call made (This is a similar analogy to a linker injecting some code from a shared library into our project).

1. First let us compile our main program: `gcc -finstrument-functions -g -c -o main.o main.c `
   1. We have passed an additional flag that leaves 'stubs' for our instrumentation with the `-finstrument-functions` flag.
2. Next, let us compile our trace file: ` gcc -c -o trace.o trace.c `
3. When we have both of our object files, we can then compile them together: `gcc main.o trace.o -o main`
4. And finally, run our program to see what you get: `./main `

### Your Deliverable

Your task now, is to take these compiler hooks and do something more useful. 

1. Find some sample c program you can modify, or continue to use the main.c provided.
2. Modify the trace.c program to count the total number of functions called.
3. Next, investigate time.h and calculate the start and stop time of each function. This will help you determine which functions you spend time in. [https://www.tutorialspoint.com/c_standard_library/c_function_difftime.htm]
4. Commit your new `trace.c` file to the repository.

# More resources to help

1. Linkers and tools https://llvm.org/docs/CommandGuide/llvm-nm.html
2. https://rafalcieslak.wordpress.com/2013/04/02/dynamic-linker-tricks-using-ld_preload-to-cheat-inject-features-and-investigate-programs/
3. http://amir.rachum.com/blog/2016/09/17/shared-libraries/#compiling-a-shared-library
4. https://jvns.ca/blog/2014/11/27/ld-preload-is-super-fun-and-easy/
5. Compiler Hooks https://balau82.wordpress.com/2010/10/06/trace-and-profile-function-calls-with-gcc/


# Going Further

(Optional) This programmer was waiting for you 9 years ago to finish this task early so you could read his 20 part linker series. [https://lwn.net/Articles/276782/]
