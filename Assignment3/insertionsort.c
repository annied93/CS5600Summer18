#include <unistd.h>

void sort(int* array, int size) {
	for(int j=1;j<size;j++) {
        int key=*(array+j);
        int i = j-1;
        while(i>-1 &&  *(array+i)>key)
        {
            *(array+i+1)=*(array+i);
            i=i-1;
        }
        *(array+i+1)=key;
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
