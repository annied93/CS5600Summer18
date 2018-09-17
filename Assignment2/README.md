# Due Date: May 23, 2018 at 11:59 PM.

TODO Please edit the following information in your assignment

- Name:An Dinh
- How many hours did it take you to complete this assignment? 5 hours
- Did you collaborate with any other students/TAs/Professors? I asked the professor some questions regarding the initialization of the array, and to check on the use of valgrind. I also discuessed with William Foster some implementation details in our program, but we did not share codes with each other. 
- Did you use any external resources? (Cite them below)
  - https://stackoverflow.com/questions/5134891/how-do-i-use-valgrind-to-find-memory-leaks/5134921
  - http://btechsmartclass.com/DS/U2_T10.html
- (Optional) What was your favorite part of the assignment? I think the challenge is how to design tests that will cover all bases. 
- (Optional) How would you improve the assignment?



# Building Data Structures - Queue
<img align="right" src="https://upload.wikimedia.org/wikipedia/commons/thumb/5/52/Data_Queue.svg/450px-Data_Queue.svg.png" alt="Queue picture">

We have previously learned the Linked List Data Structure. Now we will introduce another data structure, known as the 'queue'.

A 'queue' data structure is analogous to waiting in line at a grocery store. The first person to the checkout counter is served, followed by the next person, and then the next until the line is empty. A queue is what is known as a 'first-in first-out' (FIFO) data structure. Thus queues have a strict policy about how information is stored and accessed.

For this assignment, you are going to implement a specific implementation of a queue using an array. This data structure is also called a circular queue (specifically a ring buffer when the maximum storage size is fixed).

Provided for you is a skeleton of the implementation queue_array.c that you will implement and test.

## Part 1 - Queue implementation as an array (Ring Buffer).

A circular buffer has a fixed sized, or maximum number of elements that it can hold. This is advantageous if we know how many elements are typically stored in a data structure (If we do not know, then a queue with a linked list implemetation is needed). 

For more information on the Circular buffer, see here: https://en.wikipedia.org/wiki/Circular_buffer

This animation below gives an idea about how the data structure 'wraps' around. Think about what mathematical operator could be useful in your implementation when enqueuing and dequeuing items in your queue.

![](https://upload.wikimedia.org/wikipedia/commons/thumb/f/fd/Circular_Buffer_Animation.gif/400px-Circular_Buffer_Animation.gif)

Your task will be to implement the functions in the queue_array.c to have a working implementation of queue.

You *may* also consider writing some unit tests to test your implementation. You can include those in your submission, but we will have our own test suite.

## Part 2 - Why is a ring buffer/circular queue useful?

Queues are used quite a bit in operating systems and high performance systems, especially when performance matters. Do a little outside research, and edit test section of the Readme. Answer specifically: Why is a ring buffer useful and/or when should it be used?

(See attached text file for answer)


# Resources to help

- This is a nice interactive tutorial for learning C
  - http://www.learn-c.org/
- Queue Data Type High level description
  - https://en.wikipedia.org/wiki/Queue_(abstract_data_type)

# Rubric

- 80% Correct Queue implementation
  - All functions should be implemented
  - There should be no memory leaks
  - There should be no bugs in your functions 
  - Your implementation will be graded by our set of unit tests
- 20% Correct writeup on the importance of queues/ring buffers as related to OS
  - I expect this to be in the range of 2-3 paragraphs (Can include complexity, example usage, pros/cons, etc).

# Feedback Loop

(An optional task that will reinforce your learning throughout the semester)

- Investigate/Review more data strutures on this webpage: https://www.cs.usfca.edu/~galles/visualization/Algorithms.html
  - There are visuals for the linked list and array based queue here!

