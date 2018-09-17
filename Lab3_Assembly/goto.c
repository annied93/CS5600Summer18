#include <stdio.h>

void checkEvenOrNot(int num) {
	if (num % 2 == 0) {
		goto even;
	}
	else {
		goto odd;
	}

	even:
		printf("%d is even\n", num);
		return;
	odd:
		printf("%d is odd\n", num);
}

int main() {
	int num;
	printf("Enter a number: ");
	scanf("%d", &num);
	checkEvenOrNot(num);
	return 0;
}