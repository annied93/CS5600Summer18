#include <stdio.h>
#include <stdlib.h>
#include "binary.h"
#include <string.h>

// This is my attempt at implementing LRU that did not work. 

// ***************** CONSTANTS AND VARIABLES ***********************
// The rightmost two bits are used as the set offset value
const int cacheSetOffsetStart = 0;
const int cacheSetOffsetStop = 1;

// The tag starts at the 3rd bit to the right
const int tagStart = 2;


// Toggle these values to generate different
// addresses
#define BITWIDTH 16	// Our architecture is an 16-bit architecture. i.e. contains M= 2^m where m is BITWIDTH unique addresses.
#define ADDRESSES 16384	// By default we generate 16384 instructions
#define RANGE 256	// The range of these instructions is a value 0-RANGE. Co-incides with 'M'

#define E  4 // Number of cachelines. 4 cacheline in each of our 4 sets.
#define B  2 // Data block bits B = 2^b. 2 for example with 16 bit addresses (8 bytes per block)
#define S  4 // Number of cache sets. S=2^s. By default, start with 4.

// ******************* DATA STRUCTURES *********************
// For set 0 ~ value ending in 00
char Data1[E][BITWIDTH+1];
int Age1[E];
int counter1 = 0;
int filled1 = 0;

// Set 1 ~ value 01
char Data2[E][BITWIDTH+1];
int Age2[E];
int counter2 = 0;
int filled2 = 0;

// Set 2 ~ value 10
char Data3[E][BITWIDTH+1];
int Age3[E];
int counter3 = 0;
int filled3 = 0;

// Set 3 ~ value 11
char Data4[E][BITWIDTH+1];
int Age4[E];
int counter4 = 0;
int filled4 = 0;

// array to load the trace file into
char instructionCache[ADDRESSES][BITWIDTH+1]; 
int PC = 0;

// ********************* 

// =============== Cache Performance ================
// Keep track of how well our cache is performing with some metrics.
// Minimize the cache misses and maximize the cache hits!
int cacheHit = 0;
int cacheMisses = 0;

int costOfCacheHit = 1; // This is the latency
int costOfCacheMiss = 100; // This is latency,

int totalLatency =0;	// How fast or slow is our system

// *********************** LOAD TRACE INTO ARRAY ******************************

void loadTrace(char* filename){
	FILE* fp = fopen(filename,"r");

	char str[BITWIDTH+2]; // Strings are null terminated, so there is one extra character, plus the endline
	int pos = 0;
	while(fgets(str,sizeof(str),fp)){
		str[strlen(str)-1]='\0';		// Remove the endline character by terminating it
		strcpy(instructionCache[pos],str);	// Allocates memory for our array
		//printf("%d: %s",pos, instructionCache[pos]); // print out lines read in from file
		++pos;
	}		

	fclose(fp);
}


// *********************** PROGRESSING THROGH THE PROGRAM **************************
// This function loads an address in the simulator.
// This is equivalent to moving the program counter.
// and then we decide if it goes in the cache or not.
void stepSimulator(){
	PC++;
}


// Scorekeeping update when we have a cache miss
void incrementCacheMiss(){
	// Any time we replace we up the count
	cacheMisses++;
	// Update the total latency
	totalLatency+=costOfCacheMiss;
}

// Scorekeeping update when we have a cache miss
void incrementCacheHit(){
	// Update our count
	cacheHit++;
	// Update our latency
	totalLatency+=costOfCacheHit;
}

// *************************** DISPLAY RESULTS *******************************
void printCache(int set) {
	switch(set) {
		case 0:
			for (int i = 0; i < E; i++) {
				printf("(%d) %s ", i, Data1[i]);
			}
			break;
		case 1:
			for (int i = 0; i < E; i++) {
				printf("(%d) %s ", i, Data2[i]);
			}
			break;
		case 2:
			for (int i = 0; i < E; i++) {
				printf("(%d) %s ", i, Data3[i]);
			}
			break;
		case 3:
			for (int i = 0; i < E; i++) {
				printf("(%d) %s ", i, Data4[i]);
			}
			break;
		default:
			break;
	}
}

void drawCache(){

	//int cacheLineIndex = binaryStringRangeToInt(instructionCache[PC],cacheLineOffsetStart,cacheLineOffsetStop);
	int cacheSetIndex = binaryStringRangeToInt(instructionCache[PC],cacheSetOffsetStart,cacheSetOffsetStop);
	int tag = binaryStringRangeToInt(instructionCache[PC],cacheSetOffsetStop+1,BITWIDTH-1);

	printf("Hits: %d Misses: %d Total Latency: %d\n",cacheHit,cacheMisses,totalLatency);
	printf("Instruction[%d] is executed\n",binaryStringToInt(instructionCache[PC],BITWIDTH+1));
	
	// Note that 'set' is the cache set
	// Note that 'block' is the cache line.
	// We assume all data is valid in this cache for simplicity.
	// When the cache starts out, all of these bits are 0 however!
	printf("---------------------------\n");
	printf("valid | tag | Set | block | value\n");
	printf("1       %d    %d      -      %s   \n",tag, cacheSetIndex,instructionCache[PC]);
	printf("---------------------------\n");

	for(int i =0; i < S; i++){
		printf("Cacheset[%d]",i);
		printCache(i);
		printf("\n");
	}


	if(cacheMisses!=0){
		printf("Total Cache Hits: %d Cache Misses %d ratio %f Total Latency: %d  \n",cacheHit,cacheMisses,((float)cacheHit)/((float)cacheMisses),totalLatency);
	}
//	printf("\033[7A]");
}



// ************************ REPLACEMENT POLICY ******************************

int getMin(int array[]) {
	int i;
	int min = array[0];
	int pos = 0;
	for (i = 1; i < E; i++) {
		if (min < array[i]) {
			min = array[i];
			pos = i;
		}
	}
	return pos;
}


void addToSet1() {
	if (filled1 == E) {
	//	printf("set is full, take out pos with min age and replace\n");
		int minPos = getMin(Age1);
		strcpy(instructionCache[PC], Data1[minPos]);
		Age1[minPos] = counter1;
		counter1++;
	}
	else {
		// if set is not full
	//	printf("Set is not full, add to current position\n");
	//	printf("Position to add: %d\n", filled1);
		strcpy(Data1[filled1], instructionCache[PC]);
	//	printf("%s added\n", Data1[filled1]);
	//	printf("add the current age of this instruction: %d\n", counter1);
		Age1[filled1] = counter1;
	//	printf("Increment the counter and filled\n");
		counter1++;
		filled1++;
	//	printf("Filled position: %d\n", filled1);
	}
	return;
}

void addToSet2() {
	if (filled2 == E) {
	//	printf("set is full, take out pos with min age and replace\n");
		int minPos = getMin(Age2);
		strcpy(Data2[minPos], instructionCache[PC]);
		Age2[minPos] = counter2;
		counter2++;
	}
	else {
		// if set is not full
	//	printf("Set is not full, add to current position\n");
	//	printf("Position to add: %d\n", filled2);
		
		strcpy(Data2[filled2], instructionCache[PC]);
	//	printf("%s added\n", Data2[filled2]);
	//	printf("add the current age of this instruction: %d\n", counter2);
		Age2[filled2] = counter2;
		counter2++;
		filled2++;
	}
	return;
}

void addToSet3() {
	if (filled3 == E) {
	//	printf("set is full, take out pos with min age and replace\n");
		int minPos = getMin(Age3);
		strcpy(Data3[minPos], instructionCache[PC]);
		Age3[minPos] = counter3;
		counter3++;
	}
	else {
		// if set is not full
	//	printf("Set is not full, add to current position\n");
	//	printf("Position to add: %d\n", filled3);
		
		strcpy(Data3[filled3], instructionCache[PC]);
	//	printf("%s added\n", Data3[filled3]);
	//	printf("add the current age of this instruction: %d\n", counter3);
		Age3[filled3] = counter3;
	//	printf("Increment the counter and filled\n");
		counter3++;
		filled3++;
	//	printf("Filled position: %d\n", filled3);
	}
	return;
}

void addToSet4() {
	if (filled4 == E) {
	//	printf("set is full, take out pos with min age and replace\n");
		int minPos = getMin(Age4);
		strcpy(Data4[minPos], instructionCache[PC]);
		Age4[minPos] = counter4;
		counter4++;
	}
	else {
		// if set is not full
	//	printf("Set is not full, add to current position\n");
	//	printf("Position to add: %d\n", filled4);
		
		strcpy(Data4[filled4], instructionCache[PC]);
	//	printf("%s added\n", Data4[filled4]);
	//	printf("add the current age of this instruction: %d\n", counter4);
		Age4[filled4] = counter4;
		counter4++;
		filled4++;
	}
	return;
}

int checkInstructionPos(int set) {
	switch(set) {
		case 0:
			for (int i = 0; i < filled1; i++) {
				if (strcmp(instructionCache[PC], Data1[i]) == 0) {
					return i;
				}
			}
			break;
		case 1:
			for (int i = 0; i < filled2; i++) {
				if (strcmp(instructionCache[PC], Data2[i]) == 0) {
					return i;
				}
			}
			break;
		case 2:
			for (int i = 0; i < filled3; i++) {
				if (strcmp(instructionCache[PC], Data3[i]) == 0) {
					return i;
				}
			}
			break;
		case 3:
			for (int i = 0; i < filled4; i++) {
				if (strcmp(instructionCache[PC], Data4[i]) == 0) {
					return i;
				}
			}
			break;
		default:
			break;
	}
	return -1;
}

void replaceInstruction(int set, int pos) {
	switch(set) {
		case 0:
			strcpy(Data1[pos], instructionCache[PC]);
			Age1[pos] = counter1;
			counter1++;
			break;
		case 1:
			strcpy(Data2[pos], instructionCache[PC]);
			Age2[pos] = counter2;
			counter2++;
			break;
		case 2:
			strcpy(Data3[pos], instructionCache[PC]);
			Age3[pos] = counter3;
			counter3++;
			break;
		case 4:
			strcpy(Data4[pos], instructionCache[PC]);
			Age4[pos] = counter4;
			counter4++;
			break;
		default:
			break;
	}

}

void checkCache(int set) {

}

void replacementPolicy() {
	printf("In replacement\n");
	int set = binaryStringRangeToInt(instructionCache[PC], cacheSetOffsetStart, cacheSetOffsetStop);
	int pos = -1;
	switch (set){
		case 0:
	//		printf("In set 0\n");
			if (filled1 == 0) {
				printf("Compulsory Miss!\n");
				incrementCacheMiss();
				addToSet1();
				break;
			}
			pos = checkInstructionPos(set);
			if (pos == -1) {
				printf("Cold Miss!\n");
				incrementCacheMiss();
				addToSet1();
			}
			else {
				printf("Cache hit\n");
				incrementCacheHit();
				replaceInstruction(set, pos);
			
			}
			break;
		case 1:
	//	printf("In set 1\n");
			if (filled2 == 0) {
				printf("Compulsory Miss!\n");
				incrementCacheMiss();
				addToSet2();
				break;
			}
			pos = checkInstructionPos(set);
			if (pos == -1) {
				printf("Cold miss\n");
				incrementCacheMiss();
				addToSet2();
			}
			else {
				printf("cache hit\n");
				incrementCacheHit();
				replaceInstruction(set, pos);
			}
			break;
		case 2:
	//		printf("In set 2\n");
			if (filled3 == 0) {
				printf("Compulsory Miss!\n");
				incrementCacheMiss();
				addToSet3();
				break;
			}
			pos = checkInstructionPos(set);
			if (pos == -1) {
				printf("Cold Miss!\n");
				incrementCacheMiss();
				addToSet3();
			}
			else {
				printf("cache hit\n");
				incrementCacheHit();
				replaceInstruction(set, pos);
			}
			break;
		case 3:
	//		printf("In set 3\n");

			if (filled4 == 0) {
				printf("Compulsory Miss!\n");
				incrementCacheMiss();
				addToSet4();
				break;
			}
			pos = checkInstructionPos(set);
			if (pos == -1) {
				printf("Cold Miss!\n");
				incrementCacheMiss();
				addToSet4();
			}
			else {
				printf("cache hit\n");
				incrementCacheHit();
				replaceInstruction(set, pos);
			}
			break;
		default:
			break;
	}
}


// ************************** MAIN ******************************

int main(int argc, char** argv) {
	
	loadTrace(argv[1]);


	for (int i = 0; i < E; i++) {
		Age1[i] = -1;
		Age2[i] = -1;
		Age3[i] = -1;
		Age4[i] = -1;
	}

	char run='y';

	// We run our program until we run out of addresses to 
	// execute or the user terminates.
	while((run=='y'|| run=='a') && PC < ADDRESSES){
		printf("============================================================================\n\n");
		// (1) Check our cache and run our policy.
		replacementPolicy();
		// (2) A visual display of our cache and our user interface
		drawCache();
		// (3) We execute one instruction (Increment the PC)
		stepSimulator();

		// Ask if we would like to continue stepping through
		// our simulation.
		if(run !='a'){
			printf("Would you like to continue? (y-yes | n-no | a-simulate to finish): ");
			scanf("%s",&run);
		}
	}

	// We end our simulation
	printf("Simulation Complete\n");
	return 0;
}







