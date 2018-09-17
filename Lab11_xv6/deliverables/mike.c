#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
	mike();
	printf(1, "This is my call to mike syscall\n");
	exit();
}
