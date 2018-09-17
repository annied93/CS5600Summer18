// An Dinh - HW 2 answer

// Note: I have checked with the professor and he mentions that I don't have to initialize
// value of the array to a set default since it's easy to mistake it. I'm just going to 
// check by printing out the value of the index we are tracking as I go. 
// ==================================================
#include <stdio.h>  // For IO operations
#include <stdlib.h> // for malloc/free
#include <assert.h> // for assert

// The main data structure for the queue
struct queue{
	unsigned int back;	    // The next free position in the queue
				                  // (i.e. the end or tail of the line)
	unsigned int front;	    // Current 'head' of the queue
				                  // (i.e. the front or head of the line)
	unsigned int size;	    // How many total elements we have enqueued.
	unsigned int capacity;  // Number of items the queue can hold
	int* data; 		          // The 'integer' data our queue holds	
};
// Creates a global definition of 'queue_t' so we 
// do not have to retype 'struct queue' everywhere.
typedef struct queue queue_t;

// Create a queue
// Returns a pointer to a newly created queue.
// The queue should be initialized with data on
// the heap.
queue_t* create_queue(unsigned int _capacity){
	queue_t* myQueue = NULL;
	myQueue = (queue_t*)malloc(sizeof(queue_t));
	assert(myQueue != NULL);

	myQueue->capacity = _capacity; // set capacity to the specified value
	myQueue->front = 0; // front is 0
	myQueue->size = 0; // currently empty, so size = 0
	myQueue->back = 0;
	myQueue->data = (int*)malloc(_capacity * sizeof(int));
	return myQueue;
}

// Queue Empty
// Check if the queue is empty
// Returns 1 if true (The queue is completely empty)
// Returns 0 if false (the queue has more space available)
int queue_empty(queue_t* q){
	if (q->size == 0) {
		return 1;
	}
	return 0;
}

// Queue Full
// Check if the queue is Full
// Returns 1 if true (The queue is completely full)
// Returns 0 if false (the queue has more space available)
int queue_full(queue_t* q){
	if (q->size == q->capacity) {
		return 1;
	}
	return 0;
}

// Enqueue a new item
// i.e. push a new queue into our data structure
// Returns a -1 if the operation fails (otherwise returns 0 on success).
// (i.e. the queue is full).
int queue_enqueue(queue_t *q, int item){
	if (queue_full(q) == 1) {
		return -1;
	}
	q->data[q->back] = item;
	q->back = (q->back + 1) % q->capacity;
	q->size = q->size + 1;
	return 0;
}

// Dequeue an item
// Returns the item at the front of the queue and
// removes an item from the queue.
int queue_dequeue(queue_t *q){
	if (queue_empty(q) == 1) {
		return -1;
	}
	
	int item = q->data[q->front];
	q->front = (q->front + 1) % q->capacity;
	q->size = q->size - 1;
	return item;
}


// Queue Size
// Queries the current size of a queue
// A queue that has not been previously created will crash the program.
// (i.e. A NULL q cannot return the size.)
unsigned int queue_size(queue_t* q){
	if (q == NULL){
		printf("Queue is NULL\n");
		exit(-1);
	}
	return q->size;
}


// Free queue
// Removes a queue and all of its elements from memory.
// This should be called before the proram terminates.
void free_queue(queue_t* q){
	free(q->data);
	free(q);
	printf("Cleared\n");
}

// helper function to print out the entire queue for testing
void print_queue(queue_t* q) {
	queue_t* myQueue = q;
	for (int i = 0; i < myQueue->capacity; i++) {
		printf("%d\n", myQueue->data[i]);
	}
	printf("End of queue\n");
}

// A sample test of your program
// You can add as many unit tests as you like
// We will be adding our own to test your program.
void unitTest1(){

	queue_t* test1 = create_queue(1);
	printf("Attempting to add %d\n",1);
	queue_enqueue(test1,1);	
	print_queue(test1);
	printf("Removing: %d\n",queue_dequeue(test1));	
	print_queue(test1);

	free_queue(test1);
}

void unitTest2() {
	queue_t* test2 = create_queue(3);
	printf("Queue just created with capacity 3\n");
	print_queue(test2);
	queue_dequeue(test2);
	printf("Queue is empty? %d\n", queue_empty(test2));
	// no initialization yet, so front = 0 and back = 0
	printf("Front index: %d\n", test2->front);
	printf("Back index: %d\n", test2->back);

	
	queue_enqueue(test2,1);
	queue_enqueue(test2,2);
	queue_enqueue(test2,3);
	queue_enqueue(test2,4);

	print_queue(test2);
	// as queue is full, front is 0, back should still be 0
	printf("Front index: %d\n", test2->front);
	printf("Back index: %d\n", test2->back);

	queue_dequeue(test2);
	print_queue(test2);
	// after dequeue, front is 1, back is still 0 as it is now empty
	printf("Front index: %d\n", test2->front);
	printf("Back index: %d\n", test2->back);

	queue_enqueue(test2, 5);
	print_queue(test2);
	// front is now still 1, back is now 1 as queue is full
	printf("Front index: %d\n", test2->front);
	printf("Back index: %d\n", test2->back);

	queue_dequeue(test2);
	queue_dequeue(test2);
	printf("two dequeue's\n");
	// front is now 0, back is now 1
	printf("Front index: %d\n", test2->front);
	printf("Back index: %d\n", test2->back);

}


// ====================================================
// ================== Program Entry ===================
// ====================================================
int main(){

	// List of Unit Tests to test your data structure	
	unitTest1();
	unitTest2();


	return 0;
}
