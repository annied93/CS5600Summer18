#include <stdio.h>
#include <string.h>

#define SIZE 500000
int main() {
	char input[100000];
	char * token;
	fgets(input, sizeof(input), stdin);
	int l = strlen(input);
	input[--l] = '\0';
	//printf("%s\n", input);

	// declare delimiter
	const char delimiter[4] = " ";
	token = strtok(input, delimiter);
	//printf("%s\n", token);
	while (token != NULL) {
		if (strcmp("|", token) != 0) {
			printf("%s\n", token);
		}
		token = strtok(NULL, delimiter);
	}
	return 0;
}
