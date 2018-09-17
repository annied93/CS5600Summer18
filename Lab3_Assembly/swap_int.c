#include <stdio.h>

int main() {
	int a, b;
	printf("Enter two numbers for swapping: ");
	scanf("%d", &a);
	scanf("%d", &b);
	printf("Before swapping, the two numbers are: a = %d, b = %d\n", a, b);
	int temp = a;
	a = b;
	b = temp;
	printf("After swapping, the two numbers are: a = %d, b = %d\n", a, b);
return 0;
}
