#include <stdio.h> // Any other headers we need here
#include <malloc.h> // We bring in the old malloc function
#include <pthread.h> // for mutex


typedef struct block {
		size_t size;
		int in_use; // 0 is free, 1 is occupied
		struct block* next;
		//struct block* prev;
} block;

// keep track of the block in the linked list
block *head;
block *tail;

// define a constant blocksize
#define BLOCK_SIZE sizeof(block)

// lock access to memory when allocation occurs
pthread_mutex_t global_lock;

block *get_free_block(size_t s) {
	block *curr = head;
	while (curr) { // if curr is not null
		if (!curr->in_use && curr->size >= s) {
			// if not in used and the size is appropriate
			return curr;
		}
		// otherwise move on to the next block
		curr = curr->next;
	}
	// if cannot find the appropriate block, return NULL
	return NULL;
}

void* mymalloc(size_t s){
	size_t total_size;
	void *heap_block;
	block *header;

	if (!s) {
		// check if size if valid (not 0)
		return NULL;
	}

	// lock memory
	pthread_mutex_lock(&global_lock);
	// get a free block with the appropriate size (if available)
	header = get_free_block(s);
 
	if (header) { // if found the block
		header->in_use = 1;
		pthread_mutex_unlock(&global_lock); // unlock the mutex
		return (void*)(header + BLOCK_SIZE); // return void pointer to the memory block, excluding the header
	}
	
	// if not found an appropriate block
	total_size = BLOCK_SIZE + s; // total size needed to be allocated is the block size and the size of memory required
	heap_block = mmap(total_size); // allocated total size
	if (heap_block == (void*)-1) {
		// if allocation is unsuccessful
		pthread_mutex_unlock(&global_lock);
		return NULL;
	}
	
	// if allocation is successful
	header = heap_block;
	header->size = s;
	header->in_use = 1;
	header->next = NULL;

	if (!head) { // if the linked list is empty
		head = header;
	}
	if (tail) { // if the tail is allocated to something (linked list not empty)
		tail->next = header; // make the block just allocated the next one in the list
	}
	tail = header;
	pthread_mutex_unlock(&global_lock);

	printf("Allocated %zu bytes\n",s);
	
	return (void*)(header + BLOCK_SIZE);

}
	
void myfree(void *ptr){
	block *header;
	block *temp;
	void *programbreak;

	if (!ptr) { // if pointer is null, do not have anything to free
		return;
	}

	// lock access
	pthread_mutex_lock(&global_lock);
	header = (block*)ptr + BLOCK_SIZE;

	programbreak = mmap(0); // current heap break point
	if ((char*)ptr + header->size == programbreak) { 
	// if the current break point is equal to the break in memory 
		if (head == tail) { // head = tail -> one node in the list
			head = tail = NULL; // make null
		}
		else {
			temp = head;
			while (temp) { // if more than one node, free them
				if (temp->next == tail) {
					temp->next = NULL;
					tail = temp;
				}
				temp = temp->next;
			}
		}
		mmap(0 - BLOCK_SIZE - header->size); // get the previous location pointer
		pthread_mutex_unlock(&global_lock);
		return;
	}

	header->in_use = 0; // mark the block as free
	pthread_mutex_unlock(&global_lock);

	printf("Freed some memory\n");

}
