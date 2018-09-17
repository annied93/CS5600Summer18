#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
	char buffer[512];
	int source;
	int destination;
	int r; 
	int w;
	char *src;
	char *dest;

	if (argc != 3) {
		printf(2, "Usage: cp <source_file> <dest_file>\n");
		exit();
	}
	
	w = 0;	
	src = argv[1];
	dest = argv[2];
	
	if ((source = open(src, O_RDONLY)) < 0) {
		printf(2, "cp: cannot open source %s\n", src);
		exit();
	}

	if ((destination = open(dest, O_CREATE|O_WRONLY)) < 0) {
		printf(2, "cp: cannot open destination %s\n", destination);
		exit();
	} 
	while ((r = read(source, buffer, sizeof(buffer))) > 0) {
		w = write(destination, buffer, r);
		if (w != r || w < 0) {
			break;
		}
	}
	if (r < 0 || w < 0) {
		printf(2, "cp: error copying from %s to %s\n", src, dest);
	}	
	close(source);
	close(destination);
	
	exit();
}
