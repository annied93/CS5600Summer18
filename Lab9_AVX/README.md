# AVX

The advanced vector extensions (AVX) are an instruction set for Intel (and AMD) machines that enable parallel processing on the CPU. Today we are going to get some practice using them.

## Part 1 - Intrinsics

Intrinsics are built-in functions within a compiler. They appear as functions, and when used within a programming language, generate the appropriate assembly. Typically instrinics, or intrinsic functions, are provided to give some sort of specific optimization within a language.

The instrinsic functions for our compiler (gcc) are located in a header file named <immintrin.h>

Navigating to `/lib/gcc/x86_64-redhat-linux/4.8.5/include/immintrin.h` you can actually see the contents of this file and where various intrinsic functions live.

For AVX2 specifically, you will see:
```
#ifdef __AVX2__
#include <avx2intrin.h>
#endif
```
Navigate to <avx2intrin.h> and investigate the contents. Note, you will see within this header a message about whether or not it is okay to directly include <avx2intrin.h>. :)

## Part 2 - AVX2 Pratice

Now that you are familiar with where intrinsic instructions live, begin working through the following exercises:

* avx1.c - Uncover the basics of packing bytes into an instruction
* avx2.c - Build a print debugging function and learn about alignment.
* avx3.c - Arithmetic operations
* avx4.c - Understand masks
* avx5.c - Permuting and rearranging data with masks

For each example, you will compile the code using: (e.g. clang -std=c11 -mavx2 avx1.c -S -o avx1prog.S) *Note the -mavx2 library*

## Part 3 - AVX2 Exercises

By now you have had practice with the AVX2 instructions. Now attempt the following two exercises: ex1.c and ex2.c 

## Lab Deliverable

1. Complete ex1.c and ex2.c

## More Resources to Help

* Intel Intrinsics guide: https://software.intel.com/sites/landingpage/IntrinsicsGuide/

## Going Further

- Perf is a profiler that can return information about the performance of your program. Measuring the performance of your system is the only true way to know if code changes and optimizations are making a difference. Run through the following commands to get an introduction to perf.
  - http://www.brendangregg.com/perf.html#OneLiners
