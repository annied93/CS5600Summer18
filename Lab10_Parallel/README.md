# Lab 10 Parallel Programming

In today's lab we are going to look at some automated parallel programming tools. The best way to learn a tool like OpenMP is to read a little bit of documentation and write some examples.

Note that for today's lab, you may work locally on your machine. OpenMP is supported on the CCIS servers, but GPGPU's are not.

## Part 1 - OpenMP

### Task 1

The following guide provides some insight into using OpenMP. 

* https://bisqwit.iki.fi/story/howto/openmp/#ParallelConstruct 
* Lawrence Livermore National Labroratory https://computing.llnl.gov/tutorials/openMP/exercise.html 
* Tim's video series https://www.youtube.com/watch?v=nE-xN4Bf8XI&list=PLLX-Q6B8xqZ8n8bwjGdzBJ25X2utwnoEG&index=1

* Use these resources to create five different examples.
  * Modify omp1.c omp2.c omp3.c omp4.c omp5.c accordingly.
  * Each example should consist of a different #pragma showcasing you have read the following guide.
  * Your examples can each operate on simple loops and functions.
  * You are welcome to complete more examples if you see fit!

## Part 2 - GPGPU (Optional Section)

This next section of the lab is optional--as I cannot guarentee you have a GPU on your current machine. Most of you very likely do, even if it is integrated on your CPU!

### Task 1 -- Find your GPU

(Note this is for your local machine)

* Linux: You can try on the terminal: *lspci*, *lspci*, or look under Systems/Preferences/Hardware Information.
* Windows: Open up your Device Manager and see the Graphics Adapter installed.
* Mac: Click the Apple Icon and the Overview should show: https://www.cnet.com/how-to/how-to-find-your-macs-basic-specs-in-os-x/

### Task 2 -- Choose either CUDA or OpenCL

If you have an Nvidia graphics card available, I recommend working in CUDA. Otherwise you are restricted to using OpenCL.

### Task 3 -- Run a GPGPU Program.

Typically you have will have to download the OpenCL or CUDA Software Development Kits (SDKs). These come with samples and the drivers needed to run GPGPU programs on your machine.

- The following guide will get you started for CUDA: https://devblogs.nvidia.com/even-easier-introduction-cuda/
- The following guide will get you started for OpenCL: https://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/

## Lab Deliverable

1. Complete Part 1, task 1 and commit your 5 OpenMP examples to the repository.
2. (Optional) Build a CUDA/OpenCL Program.

## More Resources to Help

* OpenCL for MacOS https://developer.apple.com/opencl/


## Going Further

- Sign-up for an account on Northeastern's Discovery Cluster. :)
