#include <stdio.h>

int main(){
	int num1;
	int num2;

	printf("Input a starting number: ");
	scanf("%d", &num1);

	printf("Input an ending number: ");
	scanf("%d", &num2);

	if (num1 <= num2) {
		int distance = num2 - num1;
		int temp[distance+1];
		for (int i = 0; i < distance+1; i++) {
			temp[i] = num1+i;
			printf("%d\n", temp[i]);
		}
	}
	else {
		printf("Starting number is smaller than ending\n");
	}
	return 0;
}
