#include <stdio.h>

int main() {
        long int a, b;
        printf("Enter two numbers for swapping: ");
        scanf("%ld", &a);
        scanf("%ld", &b);
        printf("Before swapping, the two numbers are: a = %ld, b = %ld\n", a, b);
        long int temp = a;
        a = b;
        b = temp;
        printf("After swapping, the two numbers are: a = %ld, b = %ld\n", a, b);
return 0;
} 

