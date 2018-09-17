# Allocators

In this lab, you are going to start getting familiar with some of the tools used to implement allocators. 

Remember, part of this class is building a concrete mental model of what is really going on inside of a machine (So that there is "no more magic" when you see a computer running). We have looked at how memory is managed by the operating system and hardware, launching processes (with fork and exec), understanding a processes memory spaces, and now allocating memory--we are getting very low level!

## Setup

This lab has been tested to run on `login.ccs.neu.edu`. You should do a `git pull` on the ccis servers.

## Background Reading on Memory Allocators

Take a moment to read the Cexcerpt.pdf (2 pages ~5 minutes reading) to get a basic understanding of how allocators work. 

**Discuss** with your partner and write why you think `typedef long Align` was used. 

This depends on implementation details of the allocator. In most system, the alignment depends on word size and system restriction, but using
a long for Align is a good middle field between using an int and a double. 

## Allocator building blocks

A few of the key system calls that we will need to understand are the following (My professors use to always say "read the man pages", so I will echo that in honor of them).

* Type in `man sbrk` in the terminal. Read the description of what sbrk does .

```
BRK(2)                                                       Linux Programmer's Manual                                                      BRK(2)

NAME
       brk, sbrk - change data segment size

```

* Type in `man mmap` in the terminal next.

```
MMAP(2)                                                          Linux Programmer's Manual                                                         MMAP(2)

NAME
       mmap, munmap - map or unmap files or devices into memory
```

* Type in `man mlock` next.

```
MLOCK(2)                                                         Linux Programmer's Manual                                                        MLOCK(2)

NAME
       mlock, mlock2, munlock, mlockall, munlockall - lock and unlock memory
```

What do you think mlock is good for? This article discusses some of the tradeoffs nicely: https://eklitzke.org/mlock-and-mlockall

**Discuss** with your partner.

mlock is good for retaining necessary or sensitive instructions or data in the RAM for ease of access or for protection. 
In the attached article, two very good examples are to retain the decrypted password in RAM to make sure to preserve security, or to retain 
real-time program instructions to make sure no interruptions come during execution. 

### The simplest memory allocator

We can actually write a very simple implementation of a memory allocator. The allocator below just gives us however many bytes of memory we request. 

```

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void *malloc(size_t size) {
  void *ptr = sbrk(0);
  void *request = sbrk(size);
  if (request == (void*) -1) {
    return NULL; 
  }
  
  return ptr;
}

int main(){

	// Memory leak! But why?
	malloc(8);

	return 0;
}


```

However, the problem with this allocator is that we do not have any notion over where that memory is. We also do not really know how much memory needs to be freed(or rather, could be reused rather than asking for more memory). This means we need some sort of data structure to keep track of our heap size.

**Discuss** with your partner what data structures you might use to keep track of memory. Record at least one below, but try to think of at least two in your discussion.

One data structure that might be useful in memory tracking is a doubly linked list - which has pointers pointing to the piece of memory 
before and after it in order to keep track of where the allocated memory is located. With this struct, we can access the memory allocation
as well as the memory location before and after it. 

## Lab Deliverable

1. Fill in the 3 discussion points above in the appropriate spaces.

## More Resources to Help

N/A

## Going Further

* Take a look at malloc.c. You can skim around and read some of the comments to see how the libc library performs memory allocation!
  * https://code.woboq.org/userspace/glibc/malloc/malloc.c.html
