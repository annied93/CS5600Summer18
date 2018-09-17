// compile with: g++ -g -c hello.cpp
// investigate with: objdump -t hello.o

int foo(int a, int b) {
	return a+b;
}

int main() {
	foo(1, 2);

	return 0;
}
