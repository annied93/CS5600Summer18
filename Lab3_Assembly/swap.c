#include <stdio.h>

void swap(int* a, int* b);

int main() {
	int a, b;
	printf("Enter two numbers for swapping: ");
	scanf("%d", &a);
	scanf("%d", &b);
	printf("Before swapping, the numbers are: a = %d, b = %d\n", a, b);
	swap(&a, &b);
	printf("After swapping, the two numbers are: a = %d, b = %d\n", a, b);
return 0;
}

void swap(int* a, int* b) {
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
	 
	
