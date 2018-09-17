#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	char* temp[] = {NULL, "Test", "Program", NULL};
	temp[0] = "hello";
	execve("hello", temp, NULL);
	printf("world\n");
	return 0;
}
