#include <stdio.h>
#include <stdlib.h>

int main() {
        int* myArray = (int*)malloc(sizeof(int)*400);
        myArray[0] = 72;
        myArray[70] = 56;
        myArray[140] = 40;
        myArray[210] = 24;
        myArray[280] = 8;
        myArray[350] = 0;
return 0;
}
