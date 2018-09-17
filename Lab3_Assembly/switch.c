#include <stdio.h>

void getString(int num) {
	int div = num % 2;
	switch (div) {
		case 0:
			printf("%d is even\n", num);
			break;
		case 1:
			printf("%d is odd\n", num);
			break;
		default:
			break;
	}
}

int main() {
	int num;
	printf("Enter a number: ");
	scanf("%d", &num);
	getString(num);
	return 0;
}