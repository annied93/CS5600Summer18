# Final Project

NOTE: my partner (Aayush) uploaded a version of this already, but I guess I should upload one as well to be safe. 

Please fill out the proposal below. The range of projects for this course is wide, and can be anything Systems related (Even topics we have not covered such as databases, security, etc). The sidequests listed on the webpage may be example projects, or you may want to take a homework/lab substantially further. For your final project, think of this as a project that takes about 3x the effort of a homework. You may use other Systems languages(D, C++, Go, Rust, etc.) if it makes sense for the domain of the problem you are trying to solve.

For the final project you are expected to have a functioning tool, application, library, or framework that I can run. Additional deliverables will include a brief writeup and video demonstrating your tool in action. I see the final project as something you can be proud to put on your resume/portfolio!

## Proposal

** Fill this part out by July 3rd **

Who will you be working with (Max of 3 members--working solo is fine if you choose)?
  * Teammate 1: Aayush Jain
  * Teammate 2: Will Foster
  * Teammate 3: Annie Dinh

If you are looking for team members, you can use this spreadsheet to get in contact with folks and make a pitch.
(Your final team members information should be in the spreadsheet and in this README.md)
https://docs.google.com/spreadsheets/d/1Yg2_M6U9mx8hfAS10NH5fpNh-1u56QESIhnuEQGn1Ow/edit?usp=sharing

### In one sentence, describe your project.

We are trying to study the performance of various algorithms. We will be doing a quantitive study on various parameters such as efficiency in cache memory, execution time, and how performance varies according to the workload.

### What is the minimum deliverable for this project?

We will build a tool that will take in a binary for different algorithms and we will generate a report on the performance of these algorithms. We initially have planned to implement the following parameters:
 - Time taken
 - Time and Space Complexity Analysis
 - Cache Hits
 - Page Faults
 - Branch Misprediction

### What is the maximum deliverable for this project?

Given a number of algorithms with different work loads, we will classify the algorithms and compare each class of algorithm against one another. We will then create graphs to visually the display results of each parameter we mention above. 


### What tools or environment will you be using?

We will be using C99, gdb for debugging and test on Ubuntu 16.04


### What are your first step(s)?

First we will write a sample algorithm, make test data for it, and try to measure performance using the tools already present.


### Find at least one related publication or tool and reference it here.

https://www.openu.ac.il/personal_sites/download/galezer/efficiency.pdf
http://www.ams.org/notices/200103/fea-mcgeoch.pdf
https://ieeexplore.ieee.org/document/814600/?tp=&arnumber=814600&queryText%3Dcache%20oblivious
http://wgropp.cs.illinois.edu/projects/perf.htm

Performance analysis tools for Linux perf

### What is the biggest problem you foresee or question you need to answer to get started?

We may have to build tools to improve the accuracy of our measurements and possibly make them OS and hardware independent.


### Instructor Feedback

*Your instructor will write a short note here if this is an acceptable project.*

*Your instructor will give you some range of the grade you can earn if you meet all of your deliverables.* e.g. [B+/A-/A]

This project falls in the [B+/A-/A] range. I like the first reference listed (efficiency.pdf). Focus on making these measurements on just one system. I expect you will take some care in your experimental setup to ensure results are as accurate as possible (or otherwise be able to discuss possible threats to validity)
