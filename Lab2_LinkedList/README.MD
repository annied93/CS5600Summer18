# Linked List
"A first non-trivial data structure"

Data structures are a way for us to store information. Most programming languages come with support for an 'array' data structure for instnace, which is a data structure which stores similar(homogenous) datatypes in a contiguous piece of memory.

Here are some examples of statically allocated arrays:
```
int numbers[10];
char firstName[15];
float piDigits[5000];
```
The *statically allocated* portion means that there is a fixed number of elements in the arrays. This value is decided when we write our source code and compile it at compile-time. From the example above, we may thus only store the first 5000 digits of pi, or first names that are 15 characters or less.

Often however, we do not know how many elements need to be stored, or perhaps elements need to be removed.

For today's lab, we will be building our own data structure--the linked list. This exercise will give you practice with:

- struct's
- pointers
- programming in C

# Part 0 - SSH and git pull

First open a terminal and ssh into the CCIS servers
``` ssh ccis_user_name_here@login.ccs.neu.edu ```

Navigate to your Monorepo directory and do a ```git pull```. This will add all of the files I have previously pushed into your repositories.

# Part 1 - Understanding a struct

A struct is a user defined data type(i.e. it does not exist in C) that stores multiple pieces of data. We call this a composite data type. Here is an example of a struct in C:

```
  // The keyword 'struct' is followed by our new data type name.
  struct Student{
    int age;      // A field called 'age' that holds integer values is stored.
    int userID;   // An additional field 'userID' also holds integer values.
  };
  
  // Using the struct
  int main(){
    // We create a new struct named 'mike'
    // Note the 'struct' keyword is followed by the data type we defined above.
    struct Student mike;
    // We set the fields for our variable 'mike', which were previously not initialized.
    mike.age = 100;
    mike.userID = 123456789;
  
    return 0;
  }
```

# Part 2 - Understanding Pointers

A pointer is a variable that points to an address in memory. That is, it stores an 'address' in memory as its value.

Here are some examples in a program you can type out and run.

```
#include <stdio.h>
  
// Using the struct
int main(){
	// A regular variable	
	int x = 10;
	// A pointer that points to 
	// where 'x' is stored in memory.
	// Think of the data type as 'int*' 
	// (i.e. the asterisk or star is stuck to the data type)
	int* pointer_x = &x;
	// So if we print out the value we should get some
	// address in memory.	
	// (Note: You can google what %p is doing)
	// (	  Why do you think we had to cast the value?)
	printf("pointer_x value: %p\n",(void*)pointer_x);

	// Here we 'dereference' pointer_x to extract the value
	// of the 'thing' it points to. That is, what is the value
	// that exists at its address.
	printf("dereferencing pointer_x gives us %d\n",*pointer_x); 

	return 0;
}
```

So if a pointer can point to one value, and then point to another, do you think those values can be chained together?

# Part 3 - The Linked List

As previously discussed, the linked list is a data structure that can grow or expand. The way in which it does this is by 'pointing' to other addresses.

Conceptually a linked list is a composite data type called a 'node' that stores some data, and the location of the next node.

(Image from zentut)
<img src="http://www.zentut.com/wp-content/uploads/2013/04/linked-list-node.png" alt="Linked List visual">

From a coding perspective, it may look something like this.

```
typedef struct node{ // Note typedef saves us some typing so we do not have to type 'struct' everytime.
    int myData;
    struct node* next;
} node;	// Here is where the 'typedef' name is specified as 'node'
```

Everytime we create a node, we point the 'next' pointer to that node. This stores the location of the next node.

(Image from zentut)
(Note the first node is typically called the 'head' and the end is the 'tail')
<img src="http://www.zentut.com/wp-content/uploads/2013/04/c-linked-list.png" alt="Linked List visual">

Here is an example of a simple linked list.

```
#include <stdio.h>

typedef struct node{
	int myData;
	struct node* next;
}node;
  
// Using the struct
int main(){

	// Create a first node
	node node1;
	node1.myData = 100;
	// Create a second node
	node node2;
	node2.myData = 200;

	// Link together the nodes
	// node1 points to the address of node2
	node1.next = &node2;
	// node2 does not have a next node, so set it to 
	// NULL.
	node2.next = NULL;	

	return 0;
}

```

Now what if we want to traverse the linked list?

We can do so by using an 'iterator' pattern. That is, we use a temporary node that points to each node.

Here is an example you can try.

```
#include <stdio.h>

typedef struct node{
	int myData;
	struct node* next;
}node;
  
// Using the struct
int main(){

	// Create a first node
	node node1;
	node1.myData = 100;
	// Create a second node
	node node2;
	node2.myData = 200;

	// Link together the nodes
	// node1 points to the address of node2
	node1.next = &node2;
	// node2 does not have a next node, so set it to 
	// NULL.
	node2.next = NULL;	

	// iterating through the nodes
	node* iterator;
	// point to the first node (i.e. the head of the list)
	iterator = &node1;
	// While the iterator is not NULL, continue
	while(iterator != NULL){
		// Print myData, which is the head in the first iteration.
		printf("%d\n",iterator->myData);
		// Then move our iterator to whatever our 'next' node is in the linked list.
		iterator = iterator->next;
		// If our iterator is NULL, this loop terminates.
	}

	return 0;
}
```

# Part 4 - The task--linked list function.

Your guidelines for this task are:
1. Write a function that creates a Linked list of a user specified size
2. Populates the linked list with random numbers
3. Prints out the linked list
4. Properly deallocate (free) the linked list.

**Hints**
- What does this code do? ``` node* new_node = (node*)malloc(sizeof(node));```
- Would it be useful to make a ```node* create(int data,node* next)``` function?
  - You may use the zentut.com resource as a reference. Cite sources in your source file if you use them.


# More resources to help

- Structs: https://www.tutorialspoint.com/cprogramming/c_structures.htm
- Pointers: https://www.tutorialspoint.com/cprogramming/c_pointers.htm
- Linked List: http://www.zentut.com/c-tutorial/c-linked-list/ or https://www.cprogramming.com/tutorial/c/lesson15.html
- Typedef: https://en.wikipedia.org/wiki/Typedef

# Deliverable

- Create a file called 'linkedlist.c' demonstrating an example of a linked list following the guidelines in Part 4.
  - Additionally, add one piece of functionality into your linked list.
    - Examples
      - reverse your linked list
      - add a node at the start or end of the linked list
      - add a node in an arbitrary position of the linked list
      - Have a function that returns the number of nodes in a linked list.
