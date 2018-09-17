#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node{
    int myData;
    struct node* next;
}node;

int findLength(node* ll);

void createAndPrintLL(int specifiedSize) {
    if (specifiedSize == 0) {
        return;
    }
    
    // Create a LinkedList of a user-specified size
    node* list = (node*)malloc((int)sizeof(node)*specifiedSize + 1);

    // Seed the random number generator
    srand((unsigned) time(NULL)); // init random number generator

    // Populate the linked list with random numbers
    node* current = &list[0];
    for (int i = 0; i < specifiedSize; i++) {
        current->myData = rand() % 100;
        
        if (i < specifiedSize - 1) { 
            current->next = &list[i + 1];
            current = current->next;
        }
    }
    current->next = NULL;
    
    // Print the linked list
    node* iterator = list;
    while(iterator != NULL) {
        printf("%d\n", iterator->myData);
        iterator = iterator->next;
    }

    // Print the length of the list
    printf("Length: %i\n", findLength(list));

    // Deallocate the linked list
    free(list);
}

int findLength(node* ll) {
    int count = 0;

    node* iterator = ll;
    while(iterator != NULL) {
        count++;
        iterator = iterator->next;
    }

    return count;
}

int main(int argc, char** argv) {
    
    // Ensure an arg was passed
    if (argc == 2) {
        // Get the user-specified size
        int specifiedSize = atoi(argv[1]);
        
        // Call the funtion to create and print the list
        createAndPrintLL(specifiedSize);

    } else {
        puts("Usage: provide a size for the list.");
    }

}