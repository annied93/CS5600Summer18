#include <stdio.h>

long add(long a, long b) {
	return a + b;
}

int main() {
	long a, b;
	printf("Enter two numbers: ");
	scanf("%ld", &a);
	scanf("%ld", &b);
	long result = add(a, b);
	printf("Result is: %ld\n", result);
	return 0;
}