## Section 2 - Memory Allocator

For this assignment, you will be writing your own memory allocator. Writing a custom memory allocator is something quite likely you will do if you work on any performance sensitive system (games, graphics, quantitative finance, or just an application you want to run fast!). Malloc and Free are general purpose functions written for the average use case, but they can be optimized.

For this exercise, we are going to write our own custom memory allocator for the C language.

### Part 1 - sbrk allocator

First you will want to think about what data structure you want to use for your allocator. I typically would recommend a linked list :)

Once you are convinced of this data structure, we can begin implementing a memory allocator. 

Your objective will be to create two functions, malloc and free that are implemented in mymalloc.c. You may define any helping data structures or functions that you see fit (in fact, I recommend having some for finding free blocks, and requesting blocks). A `compile.py` file and `run.py` file is also provided for you.

1. First run `python compile.py` to make sure everything builds by default.
2. Then run `python run.py` to make sure everything runs (`run.py` also compiles the code).
3. You will notice in the tests directory, some small tests have been created for you. test1, test2, etc. use the default implementation of malloc. test1_mymalloc, test2_mymalloc, etc. use your custom implementation of malloc and free. You will notice in the compile script, the use of compile-time interpositioning to replace the default C memory allocator.
4. Okay, now you are ready to begin implementing malloc. Some details on the specifications are below.

#### Your task is to implement malloc and free using a first-fit memory allocator.

Let me give you some tips on how to do this.

1. You should have a linked list data structure. This data structure will keep track of the blocks that you have allocated within your malloc function. You should have a global variable that serves as the "head" or "first block of memory" in your linked list.
2. You should have some notion of a 'block' of memory in your program.
3. An example is provided here with some fields that may be useful:

```
  struct block{
    size_t size; // How many bytes beyond this block have been allocated in the heap
    block* next; // Where is the next block
    int free; // Is this memory free?
    int debug; // (optional) Perhaps you can embed other information--remember, you are the boss!
  };
```

4. You will want to keep track of how big this block structure is. A little trick is to use the preprocessor so you can simply use BLOCK_SIZE in your code.

```
#define BLOCK_SIZE sizeof(block)
```

5. The `sbrk` command will be your friend. Understand what sbrk(0) does and sbrk(10) before getting started. Your malloc function (or helper functions) are what will use malloc.

6. Your free function can be as simple as setting a block of memory to be free. Consider how memory is laid out in the heap however to make sure you are only accessing your struct. Here is a simple diagram: 

|block|----actual memory----|block|--------actual memory-------|block|--actual memory--|

  ^ Here is where your struct lives, this is what you want to update.

7. A first-fit memory allocator means, we are looking for any block in our linked list that is free (any previously allocated blocks that is). Remind yourself what the trade-off is between the other allocators (e.g. compare to a 'best-fit' allocator).

8. Your malloc function should print out "Allocated %zu bytes\n"
9. Your free function should print out "Freed %zu bytes\n"

10. With these print outs, you can see if they match the original programs.
11. I will look through your code to make sure you do not use malloc from <stdlib.h>. You should only be using syscalls such as sbrk (you may also use mmap if desired).

## Rubric

- 50% for Memory Allocator
  - You should have a working allocator that does not leak memory.
     
# Resources to help
- Advanced Material: https://www.youtube.com/watch?v=kSWfushlvB8 ("How to Write a Custom Allocator" for C++)
- The book Programming in C (K&R book) has a nice description of memory allocators.

### Glossary and Tips
1. Commit your code changes to the repository often, this is best practice.
2. Do not commit your .o file or your executable file to the repository, this is considered bad practice!
3. On Functionality/Style
	1. You should have comments in your code to explain what is going on, and use good style (80 characters per line maximum for example, functions that are less than 50 lines, well named variables, well named functions etc.).

# Going Further
Your do not have to align your memory, but you may choose to (I will only run test on nicely sized memory blocks).

Blocks of memory typically are aligned. That is, they are aligned to some address boundary (every 8, 16, 32, 64, etc.). In order to make our memory allocator more simple, we can force alignment of every block. Here is a trick using a union from the K&R book.

```
typedef long Align; // Alignment to long size

union header{
  struct{
    union header *ptr;  // next block in free list
    unsigned size;      // Size of this block
  } s;
  Align x;  // Force alignment of blocks to our boundary.
            // Note this field is never used, but space is allocated for it.
            // Convince yourself this 'trick' works and why.
};
```


Other Notes:

* In order to avoid fragmentation, most allocators combine blocks that are adjacent(on both sides) into bigger ones. They may then split those bigger blocks as need as well. You could write some helper functions to assist with this, and have a more optimal memory allocator. 
* The malloc implementation is actually open source, so you can take a look at it if you are curious.

# Feedback Loop

(An optional task that will reinforce your learning throughout the semester)

Not applicable, as we will do more with allocators later!
