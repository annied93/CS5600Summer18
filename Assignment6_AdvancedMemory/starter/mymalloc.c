#include <stdio.h> // Any other headers we need here
#include <malloc.h> // We bring in the old malloc function
#include <pthread.h> // for mutex
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>

#define NUM_PROC sysconf(_SC_NPROCESSORS_ONLN)
#define PROCS 32 // since ccis machines has 32 cores 

typedef struct block {
	size_t size;
	int in_use; // 0 is free, 1 is occupied
	struct block* next;		
} block;

typedef struct list {
	size_t size;
	block * head;
	block * tail;
} list;

typedef struct arena {
	// int used_so_far;
	list* block_64;
	list* block_256;
	list* block_1024;
	list* over_1024_block;
	pthread_mutex_t lock;				
} arena;

// initialize the arena array free_lists
arena free_lists[PROCS];

// initialize the flags to check if arena arrays are initialized
int arena_initializer = 0;
int size_mmap_list = 0;

// define a constant blocksize
#define BLOCK_SIZE sizeof(block)
#define LIST_SIZE sizeof(list)
#define HEAP_PAGE_SIZE sysconf(_SC_PAGE_SIZE)
#define NUM_64_BLOCKS 1024
#define NUM_256_BLOCKS 512
#define NUM_1024_BLOCKS 64
#define TOTAL_ALLOCATION 262144
#define BLOCK_64_TOTAL NUM_64_BLOCKS * (64 + BLOCK_SIZE)
#define BLOCK_256_TOTAL NUM_256_BLOCKS * (256 + BLOCK_SIZE)
#define BLOCK_1024_TOTAL NUM_1024_BLOCKS * (1024 + BLOCK_SIZE) 

// lock access to memory during the initialization
pthread_mutex_t global_lock;
int g_lock_init = 0;

block *get_free_block(size_t s, arena free_list) {
	block *curr;
	printf("Find block for size %zu\n", s);
	if (s <= free_list.block_64->size) {
		curr = free_list.block_64->head;
	}
	else if (s <= free_list.block_256->size) {
		curr = free_list.block_256->head;
	}
	else if (s <= free_list.block_1024->size) {
		curr = free_list.block_1024->head;
	}
	else {
	//	printf("Block size belong to list 3\n");
		curr = free_list.over_1024_block->head;
	}

	while (curr) {
		//printf("iterating through list now\n");
		if (curr->in_use == 0  && curr->size >= s) {
		//	printf("found block, now return\n");
			return curr;
		}
		//printf("not found, now move on\n");
		curr = curr->next;
	}
	//printf("Block not found, need to allocate new\n");
	return NULL;
}


void initialize_arena() {
//	printf("init arena\n");
	block *curr;
	void *heap_block;
	for (int i = 0; i < PROCS; ++i) {
	//	free_lists[i] = mmap(NULL, TOTAL_ALLOCATION,, PROT_EXEC|PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		//free_lists[i].used_so_far = 0;


// need to allocate memory to each of the list and then change it accordingly from there.
		//printf("Init block_64: Allocate list size memory\n");
		free_lists[i].block_64 = sbrk(LIST_SIZE);
		//printf("Set size to 64\n");
		free_lists[i].block_64->size = 64;
		//printf("Start to init each block\n");
		for (int j = 0; j < NUM_64_BLOCKS; ++j) {
		//	printf("Init block now\n");
			heap_block = sbrk(BLOCK_SIZE + 64);
			if (heap_block == (void*)-1) {
				printf("Init fail, exit\n");
				return;
			}
			
			curr = heap_block;
			curr->size = 64;
			curr->in_use = 0;
			curr->next = NULL;
			if (free_lists[i].block_64->head == NULL) {
				free_lists[i].block_64->head = curr;
			}
			if (free_lists[i].block_64->tail != NULL) {
				free_lists[i].block_64->tail->next = curr;
			}
			free_lists[i].block_64->tail = curr;
		}	
		free_lists[i].block_256 = sbrk(LIST_SIZE);
		free_lists[i].block_256->size = 256;
		for (int k = 0; k < NUM_256_BLOCKS; ++k) {	
			heap_block = sbrk(BLOCK_SIZE + 256);
			if (heap_block == (void*)-1) {
				return;
			}
			curr = heap_block;
			curr->size = 256;
			curr->in_use = 0;
			curr->next = NULL;	
			if (free_lists[i].block_256->head == NULL) {
				free_lists[i].block_256->head = curr;
			}
			if (free_lists[i].block_256->tail != NULL) {
				free_lists[i].block_256->tail->next = curr;
			}
			free_lists[i].block_256->tail = curr;
		}
		free_lists[i].block_1024 = sbrk(LIST_SIZE);
		free_lists[i].block_1024->size = 1024;
		for (int n = 0; n < NUM_1024_BLOCKS; ++n) {	
			heap_block = sbrk(BLOCK_SIZE + 1024);
			if (heap_block == (void*)-1) {
				return;
			}
			curr = heap_block;
			curr->size = 1024;
			curr->in_use = 0;
			curr->next = NULL;
			if (free_lists[i].block_1024->head == NULL) {
				free_lists[i].block_1024->head = curr;
			}
			if (free_lists[i].block_1024->tail != NULL) {
				free_lists[i].block_1024->tail->next = curr;
			}
			free_lists[i].block_1024->tail = curr;
		}
		free_lists[i].over_1024_block = sbrk(LIST_SIZE);
		free_lists[i].over_1024_block->size = 1025;
		free_lists[i].over_1024_block->head = NULL;
		free_lists[i].over_1024_block->tail = NULL;
		if (pthread_mutex_init(&(free_lists[i].lock), NULL) != 0) {
			printf("Mutex initializer error\n");
		}
	}	
}

void* get_block(size_t size) {
	pthread_mutex_lock(&global_lock);
	void* heap_block = sbrk(size);
	if (heap_block == (void*)-1) {
		pthread_mutex_unlock(&global_lock);
		return (void*)-1;
	}
	pthread_mutex_unlock(&global_lock);
	return heap_block;
}

void* mymalloc(size_t s){
	if (g_lock_init == 0) {
		pthread_mutex_init(&global_lock, NULL);
		g_lock_init = 1;
	}
	//printf("starting malloc - init the array of arenas\n");
	pthread_mutex_lock(&global_lock);
	if (arena_initializer == 0) {
		initialize_arena();
		arena_initializer = 1;
	}
	pthread_mutex_unlock(&global_lock);
		
	//printf("run malloc\n");
	size_t total_size;
	void *heap_block;
	size_t size_allocated;
	block *header;
	int order = -1;
	//printf("Get current process number\n");
	int curr_proc = get_nprocs() % PROCS;
	
	//printf("access free list %d\n", curr_proc);
	arena free_list = free_lists[curr_proc];
	
	if (!s) {
		// check if size if valid (not 0)
		return NULL;
	}

	// lock memory
	int ret = pthread_mutex_lock(&(free_list.lock));
	//printf("%p lock address\n", &(free_list.lock));
	if (ret != 0) {
		printf("locking fail\n");
	}

	// get a free block with the appropriate size (if available)
//	printf("Going to find if the block exists\n");
	header = get_free_block(s,free_list);
	
	if (s <= free_list.block_64->size) {
	//	printf("Block size smaller than 64 so list 0\n");
		order = 0;
		if (header != NULL) { // if found the block
			//printf("Block found\n");
			header->in_use = 1;
			pthread_mutex_unlock(&(free_list.lock)); // unlock the mutex
			printf("Allocated %zu bytes\n", header->size);
			return (void*)(header + BLOCK_SIZE); // return void pointer to the memory block, excluding the header
		}
		// if not found an appropriate block
		total_size = BLOCK_SIZE + free_list.block_64->size;
		heap_block = get_block(total_size);
		header = heap_block;
		header->size = 64;
		header->in_use = 1;
		header->next = NULL;
	}
	else if (s <= free_list.block_256->size) {
		order = 1;
		if (header != NULL) { // if found the block
			header->in_use = 1;
			pthread_mutex_unlock(&(free_list.lock)); // unlock the mutex
			printf("Allocated %zu bytes\n", header->size);
			return (void*)(header + BLOCK_SIZE); // return void pointer to the memory block, excluding the header
		}
		// if not found an appropriate block
		total_size = BLOCK_SIZE + free_list.block_256->size;
		heap_block = get_block(total_size);

		if (heap_block == (void*)-1) {
		// if allocation is unsuccessful
			pthread_mutex_unlock(&(free_list.lock));
			return NULL;
		}
		header = heap_block;
		header->size = 256;
		header->in_use = 1;
		header->next = NULL;
	}
	else if (s <= free_list.block_1024->size) {
		order = 2;
		if (header != NULL) { // if found the block
			header->in_use = 1;
			pthread_mutex_unlock(&(free_list.lock)); // unlock the mutex
			printf("Allocated %zu bytes\n", header->size);
			return (void*)(header + BLOCK_SIZE); // return void pointer to the memory block, excluding the header
		}
		// if not found an appropriate block
		total_size = BLOCK_SIZE + free_list.block_1024->size;
		heap_block = get_block(total_size);
		if (heap_block == (void*)-1) {
		// if allocation is unsuccessful
			pthread_mutex_unlock(&(free_list.lock));
			return NULL;
		}

		header = heap_block;
		header->size = 1024;
		header->in_use = 1;
		header->next = NULL;
	}
	else {
		order = 3;
		total_size = BLOCK_SIZE + s; // total size needed to be allocated is the block size and the size of memory required
		heap_block = mmap(0, total_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	//	printf("Allocated heap block\n");
		if (heap_block == MAP_FAILED) {
		// if allocation is unsuccessful
			pthread_mutex_unlock(&(free_list.lock));
			return NULL;
		}
		//printf("set the heap block to the new list now\n");
		header = heap_block;
		//printf("set header size to be %zu\n", s);
		header->size = s;
		//printf("set header in use\n");
		header->in_use = 1;
		header->next = NULL;
		//++size_mmap_list;
	///printf("add a block to the mmap list - total size %d\n", size_mmap_list);
	}
	// if allocation is successful
	

	if (order == 0) {
		if (free_list.block_64->head == NULL) {
			free_list.block_64->head = header;
		}
		if (free_list.block_64->tail != NULL) {
			free_list.block_64->tail->next = header;
		}
		free_list.block_64->tail = header;
		pthread_mutex_unlock(&(free_list.lock));
	}
	else if (order == 1) {
		if (free_list.block_256->head == NULL) {
			free_list.block_256->head = header;
		}
		if (free_list.block_256->tail != NULL) {
			free_list.block_256->tail->next = header;
		}
		free_list.block_256->tail = header;
		pthread_mutex_unlock(&(free_list.lock));
	}
	else if (order == 2) {
		if (free_list.block_1024->head == NULL) {
			free_list.block_1024->head = header;
		}
		if (free_list.block_1024->tail != NULL) {
			free_list.block_1024->tail->next = header;
		}
		free_list.block_1024->tail = header;
		pthread_mutex_unlock(&(free_list.lock));
	}
/*
	else {	
		if (free_list.over_1024_block->head == NULL) {
			free_list.over_1024_block->head = header;
		}
		if (free_list.over_1024_block->tail != NULL) {
			free_list.over_1024_block->tail->next = header;
		}
		free_list.over_1024_block->tail = header;
*/		int ret_unlock = pthread_mutex_unlock(&(free_list.lock));
		//printf("%p unlock\n", &(free_list.lock));
		if (ret_unlock != 0) {
			printf("unlock unsuccessful\n");
		}
	
	
	printf("Allocated %zu bytes\n",header->size);
	
	return (void*)(header + BLOCK_SIZE);
}
	
void myfree(void *ptr){
	block *header;
	block *curr;
	//void *programbreak;

	
	if (ptr == NULL) { // if pointer is null, do not have anything to free
		return;
	}
		
	int curr_proc = get_nprocs() % PROCS;
	arena free_list = free_lists[curr_proc];
	
	// lock access
/*	if (pthread_mutex_lock(&(free_list.lock)) != 0) {
		printf("lock unsuccessful\n");
	} */
	header = (block*)ptr - BLOCK_SIZE;
		
	//s = header->size;
	//printf("need to find block size %zu to free\n", header->size);
	//printf("address to find %p\n", header);
	//if (header->size > 1024) {
	//	printf("Freed %zu bytes\n", header->size);
	//	header->in_use = 0;
		//munmap(header, header->size + BLOCK_SIZE);
		//printf("Freed %zu bytes\n", header->size);
	

	if (header->size <= free_list.block_64->size) {
		//printf("block to find in list_64\n");
		curr = free_list.block_64->head;
	}
	else if (header->size <= free_list.block_256->size) {
		//printf("block to find in list_256\n");
		curr = free_list.block_256->head;
	} 
	else if (header->size <= free_list.block_1024->size) {
		//printf("block to find in list_1024\n");
		curr = free_list.block_1024->head;
	}	
	else {
		header->in_use = 0;
		printf("Freed %zu bytes\n", header->size);
		munmap(header, header->size + BLOCK_SIZE);
		pthread_mutex_unlock(&(free_list.lock));
		return;
		//curr = free_list.over_1024_block->head;
	}
	
	while (header != curr && curr != NULL) {
		curr = curr->next;
	}
	if (curr == header) {
		//printf("Found block to free\n");
		curr->in_use = 0;
		
		header->in_use = 0; // mark the block as free
		if (pthread_mutex_unlock(&(free_list.lock)) != 0) {
			printf("Unlock unsuccessful\n");
		}
		printf("Freed %zu bytes\n", header->size);
		return;
	}
/*	else if (curr == header && order3 == 1) {
		printf("Block is in mmap list\n");
		//munmap(header, header->size + BLOCK_SIZE);
		pthread_mutex_unlock(&free_list.lock);
		printf("Freed %zu bytes\n", s);
		return;
	} */
	else {
		if (pthread_mutex_unlock(&(free_list.lock)) != 0) {
			printf("Unlock fails\n");
		}
	}
	return;
}
