// For this exercise, you are going to read in data from a file called 'data.txt'
// Increment all of the values by 1.
// 
// You will compile this source using the following:
// 
// clang -std=c11 -mavx2 ex1.c -o ex1prog
//
// Run as normal with: 
// 
// ./exx1prog
//
#include <stdio.h>
#include <immintrin.h>

#define FILE_SIZE 32
#define BLOCK 8

// print
void print__m256(__m256 data){
	float*f = (float*)&data;
	printf("%f %f %f %f %f %f %f %f\n", f[0],f[1],f[2],f[3],f[4],f[5],f[6],f[7]);
}	

int main(){
	char* file_name = "data.txt";
	FILE* file = fopen(file_name, "r");
	if (file == NULL) {
		printf("Error in opening file\n");
	}
	float arr[32];
	int i = 0;
	int j = 0;
	fscanf(file, "%d", &i);
	while (!feof(file)) {
		arr[j]= i;
		fscanf(file, "%d", &i);
		++j;
	}
	fclose(file);

	
	__m256 data1 = _mm256_set_ps(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7]);
	__m256 data2 = _mm256_set_ps(arr[8], arr[9], arr[10], arr[11], arr[12], arr[13], arr[14], arr[15]);
	__m256 data3 = _mm256_set_ps(arr[16], arr[17], arr[18], arr[19], arr[20], arr[21], arr[22], arr[23]);
	__m256 data4 = _mm256_set_ps(arr[24], arr[25], arr[26], arr[27], arr[28], arr[29], arr[30], arr[31]);

	// standard file of ones to be added for the incrementation of values	
	__m256 ones = _mm256_set1_ps(1.0);

	__m256 data1_incr = _mm256_add_ps(data1, ones);
	__m256 data2_incr = _mm256_add_ps(data2, ones);
	__m256 data3_incr = _mm256_add_ps(data3, ones);
	__m256 data4_incr = _mm256_add_ps(data4, ones);

	print__m256(data1_incr);
	print__m256(data2_incr);
	print__m256(data3_incr);
	print__m256(data4_incr);
	return 0;
}
