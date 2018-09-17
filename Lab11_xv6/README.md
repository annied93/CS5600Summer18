# Lab 11 xv6

<img src="https://images.techhive.com/images/idge/imported/article/ctw/2009/06/04/unix_chart_775-100397147-orig.jpg" alt="Unix operating systems" width="400px" align="right"> In today's lab we are going to be exploring and working in the xv6 operating system. This is a Unix inspired operating system that is a manageable size to work with in a lab (The actual Linux Kernel is several millions lines of C code!). That being said, this is still a large piece of software, and getting experience working with mid to large sized source code you are not familiar with is an important skill!



The overall taks are:

- Install an emulator
- Download xv6 for github, and build an img file using 'make'
- Modify an unfamiliar piece of source code (i.e. xv6)

## Task 1 - Install an emulator and Build xv6

For this lab we need to install an emulator that can emulate an operating system. Because we do not want to dedicate our personal machines to installing xv6 (although we could!). You will do this locally on your machine.

1. The first step is to install QEMU by navigating to https://www.qemu.org/download/#linux. You may follow the directions here, or I have highlighted them below for several platforms.

### Linux

- Run the following on the terminal: `sudo apt-get install qemu`

See: https://www.qemu.org/download/#linux for the official instructions.

### Windows 7 or earlier.

(If you have Windows 10, you can skip these steps and navigate below to 'Install xv6')

- Download the appropriate binary from: https://www.qemu.org/download/#windows
  - The exact download I used for my Windows x86_64 machine was: https://qemu.weilnetz.de/w64/2018/qemu-w64-setup-20180711.exe
  - Run the installer with the default settings.
  - You can launch the executable to test it out (e.g. from C:\Program Files\qemu\qemu-system-x86_64w.exe)
    - However, we do not have xv6 installed quite yet! Keep reading.
    
See: https://www.qemu.org/download/#windows for the official instructions.

### Mac

- You will need either 'brew' or 'ports' to install QEMU
  - I recommend installing brew which can be done here: https://brew.sh/
  - Close and then open your terminal after installing.
  - Then run: 'brew install qemu' from a terminal.

See: https://www.qemu.org/download/#macos for the official instructions.

### Building from source

If none of the above options work, you can try to build from source for your architecture. This is a good skill to have, but may take some time and additional configuring. See (https://www.qemu.org/download/#source) for directions.

## Task 2 - Install xv6

(The official instructions are here: https://pdos.csail.mit.edu/6.828/2017/xv6.html)

We are going to run xv6 in an emulator--specifically the QEMU emulator that you built above. What this means is we now need to compile the xv6 operating system into a format that our emulator understands. If you are using linux, this is relatively straight forward. If you are not using Linux, then we need to 'cross-compile' the binary.

Cross-compiling means to generate a binary file that is not necessarily for our native platform. That is, if you are windows normally when you compile, executables are generaetd that run on windows. So what is different if we are on windows, is we want to generate a binary that actually is of the ELF format (which windows does not understand, but our emulator will).

Proceed to the directions below.

### Obtaining xv6

xv6 lives in a github repository for the MIT operating systems class. 

- Obtain the repository here: `git clone git://github.com/mit-pdos/xv6-public.git`

### Building xv6 on Linux

1. Once the repository has downloaded run navigate inside the `public-xv6` folder.
2. Type `make` within the public-xv6 repository you have downloaded and wait a few seconds for the OS to compile. 

### Building xv6 on Mac

- I recommend trying this automatic script from: https://gcallah.github.io/OperatingSystems/xv6Install.html (You then simply run `bash install_cross.sh`)
- If there are errors, you may follow more specific instructions here: https://doesnotscale.com/compiling-xv6-on-os-x-10-9/
- Otherwise the general steps are below.

1. Install gcc (Clang will not work)
2. Once the repository has downloaded run navigate inside the `public-xv6` folder.
3. Within the xv6 dirctory you have cloned, type: 'make TOOLPREFIX=i386-jos-elf-'
4. Install any missing tools needed, and retry the make command.

### Building xv6 on Windows

If you have Windows 10 64-bit follow the directions below.

- Follow the instructions here: https://gcallah.github.io/OperatingSystems/xv6Install.html (I have also written them below)

1. From within Bash on on windows(https://www.howtogeek.com/249966/how-to-install-and-use-the-linux-bash-shell-on-windows-10/) type the following: `sudo apt-get update && sudo apt-get install git nasm build-essential qemu gdb`
2. Then within the MakeFile in the directory modify `QEMUOPTS = -hdb fs.img xv6.img -smp $(CPUS) -m 512 $(QEMUEXTRA)` **to** `QEMUOPTS = -hdb fs.img xv6.img -smp $(CPUS) -m 512 $(QEMUEXTRA) -display none`

If you have Windows 7 or earlier, you will need to install cygwin or some other terminal, and follow similar instructions to the Mac.

## After building xv6 -- Actually running xv6

Okay great, what did we achieve? After building xv6 you should see the `fs.img and xv6.img` This is an 'image' of your operating system. In other words, you whole operating system wrapped up into a single file. The emulator (qemu) will then boot up the operating system from within this image file, and host it in a virtualized environment.

Okay, final step from whatever terminal you are working in.

- If you are working in Unix, or on Mac, it should be as simple as typeing `make qemu-nox` and the operating system will begin in your current shell! (If for some reason that does not work, you can try `make qemu`)
- If you are running in the Windows Linux subsystem run: `make qemu fs.img`

### Grep'ing around to learn part 1

Remember the `grep` command? (man grep) It is a handy way to search text, and also a great way to navigate great big projects. So if you have a large project like xv6, where to start? How about finding the main method?

- If I was grep'ping around the source I would likely start with something like: `grep -ir -e "main(" .`
  - 'i' means case-insensitive. 'r' means do a recursive search 'e' is the pattern 'main(' which I am looking for, and the '.' at the end tells me where to start this search.
  - After this investigation, I will find `main.c`. Starting from main.c is always a good way to start understanding a programs control flow.

## Getting familiar with xv6.

While running the emulator type 'ls' and you will see all of the commands you have available.

Now that is not quite as many as we have available in Unix--but it is a start!

## Task 2 - Extending xv6

Remember the applications like ls, wc, cat, etc. are all programs that someone has written and compiled as a binary. Let's add a simple binary to our xv6 operating system to see how this works in the build process.

- Make sure you have exited the emulator.
- Within the root directory of the public-xv6 directory you will see all binaries for different programs and their .c files.
- Let's emulate the style of command by copying a previously created file: `cp echo.c mycommand.c`

Here is the code for our command:

```
// Three files are included.
// None of which are the usual
// '<stdio.h>' we would use. 
#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    
    int i;
    for(i = 1; i < argc; i++){
        // Remember what the '1' is as the first argument?
        printf(1, "%s%s", argv[i], i+1 < argc ? " " : "\n");
    }
    // An exit call rather than return statement    
    exit();
}
```

- Now in order for this to be integrated into the build process, we need to modify our 'MakeFile' file.
- Enter into vim and modify the following
  - Under UPROGS add: `_mycommand\` (Emulate the exact style, do not forget the slash at the end)
  - Under EXTRA add: 'mycommand.c'
  - Now type 'make' and repeat your build process you did earlier to rebuild everything.
    - Note that 'make' is typically smart enough to only rebuild the files that it has to.

- You can now do a test by launching xv6, and then typing 'ls'. You should see your command listed.
- Now try typing `mycommand test` and you should see the same output you would as if you did 'echo'
- We can begin appreciating the work that has gone into our modern operating systems!

### Grep'ing around to learn part 2

- Now investigate each of the header files, types.h, stat.h, and user.h in vim. Take a few minutes to see what you find.
- What specifically do you find in user.h that is interseting?

## Task 3 - Implementing a System Call (Extending xv6 further).

(Reference Chapter 3 for more: https://pdos.csail.mit.edu/6.828/2014/xv6/book-rev8.pdf)

We have seen conceptually how system calls worked, and even used them in assembly(e.g. write) and our C code (e.g. brk). When we make these system calls, our user programs are requesting the kernel to perform some action. In order for the kernel to perform this action, a context switch occurs, the action is performed, and then the user program gets control again.

Recall at a high level what happens during a system call.

1. A trap is issued. This stops the process from running.
2. The process needs its registers saved.
3. We switch to kernel mode and perform work.
4. Restore our registers with any applied changes as we give the user program back control.


### Getting started

Let's first take a look at syscalls that are already implemented and walk through. The below is my work process and how I 'play around' when in unfamiliar territory.

- I sort of have a hunch where these will be, but I grep anyway `grep -ir -e "sys" --include="*.h" .` this time looking in the header file for some hints.
- I see syscall.h looks interseting, so I will open that file.
  - Modify syscall.h and add a syscall like:
  -  `#define SYS_mike 22`
- At this point, I might look in syscall.c the corresponding file to our header file.
  - After browsing this file I notice a few things.
    - One is that the comments at the top of the file discuss a requirement to save program counter.
    - The second is there are a few helper functions for working with the system call.
    - The third is there is a list of function prototypes, and a jump table with function pointers we might need to add to.
    - Let's go ahead and add to syscall.c
      - `extern int sys_mike(void)`
      - `[SYS_mike]  sys_mike, // This line in the list of function pointers`
- Now at this point, I am almost completely lost. Let's continue grep'ping around.
- Before Let's look at the other header files within syscall.c starting with types.h
  - types.h -- nothing too interseting in here. Let's move along
  - defs.h -- ah, okay some interseting stuff here. More lists of things, maybe their are other system calls here?
    - In fact here, I will add `int      mike(void); // under proc.c which falls around line 123.`
- Let us continue our tour from syscall.h and next open up param.h
  - This file shows us some more interesting configurations of our xv6 operating system.
- Let's do some more grep and see where we need to add our system call.
  - 'wait' is another system call that I know of and it fell under proc.c (and it matches our functions signature). What I really want to find is its function prototype, or where it is defined.
  - I can look back at defs.h and see the form.
  - I try: `grep -ir 'int wait(.*);' .` and find users.h has definitions.
  - Surely enough when I look in user.h, I see in the comments 'system calls' (Ah--I love when folks comment their code!)
    - Add `int mike(void);` to the list.
    - Okay, now I want to figure out where to implement 'mike' (or whatever you named your sys_call).
      - I'm going to look to *wait* again, and try `grep -ir "wait(*.*)" .` with my goal being to find the actual implementation.
      - I get lots of hits, so I narrow this down slightly.
        - Knowing that an implementation is a .c file, I try: `grep --include="*.c" -ir 'wait(*.*)' .`
        - At this point I make an educated guess that the usertest.c file is not useful. As I browse up I do see a prototype, and see it is of the form sys_wait(voiid) in syscall.c. I also see sysproc.c has this same sys_mysystemcall pattern as well.
          - One more grep just to be sure: `grep --include="*.c" -ir 'sys_wait(*.*)' .`
      - In sysproc.c add 
      ```
      int sys_mike(void){
          return mike();
      }
      ```
      - In syscall.c we already did everything--(oops I should have remembred the sys_mysyssetmcall pattern...)
      
Okay, so we have done quite a bit so far, but we are not *quite* done.

- During my greps I have noticed `SYSCALL(wait)` appears quite often. After more investigation, usys.S is the responsible file that needs modified.
  - Add `SYSCALL(mike)` at the end.
- Finally after one more grep `grep -ir 'wait(void)' .` we see that proc.c is where we can implement our command.
  ```
    int
  mike(void){
      // Obtain process structure
      // struct proc* p;
      // Perhaps do something interesting?

      // Actually..let's just print hello
      cprintf("hello from mike syscall\n");

      return 22; // Our system call number
  }
  ```
  
Okay, now we just need to make a program that uses our syscall. Let's create mike.c and add it to our MakeFile as we did before. We will need to rerun 'make' with whatever parameters we previously used. Then just run our user program that makes the system call. 
  
(Note: While it is an honor to have a syscall named 'mike', sometimes naming things weird also makes them easy to grep for when first learning. Our worse case scenario here if I really mess something up is to download xv6 again.)

## Lab Deliverable

- Implement the 'cp' as a user program.
  - Note you will need to include 'fctnl.h' in your implementation.
  - Why? open the file to see why. :)
- Implement a syscall 'mike' or whatever you choose with a test program.

## More Resources to Help

* xv6 is fairly widely used for operating systems exercisese. Googling around for install trouble will likely yield a working result.
* If you want to debug xv6 while developing, you can type `make-qemu-gdb` When launching this will give you a process id that you can attach to (You will see it as tcp:12345, where 12345 is shown). Then launch gdb. Then type `target gdb remote:12345`
* More on grep: https://www.gnu.org/software/grep/manual/html_node/Regular-Expressions.html


## Going Further

- Implement a syscall that prints out process information.
