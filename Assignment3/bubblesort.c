//bubble sort

#include <unistd.h>

void swap (int* a, int*b) {
	int t = *a;
	*a = *b;
	*b = t;
}

void sort(int* array, int size) {
	int i, j;
	for (i = 0; i < size-1; i++) {
		for (j = 0; j < size-i-1; j++) {
			if (*(array+j) > *(array+j+1)) {
				swap(array+j, array+j+1);
			}
		}
	}
}

int main() {
	int data[9] = {116, 67, 69, 102, 101, 102, 111, 114, 65};

	sort(data, 9);
	int i = 0;
	for (i = 0; i < 9; ++i) {
		write(1, &data[i], sizeof(data[i]));
	}

	return 0;
}
