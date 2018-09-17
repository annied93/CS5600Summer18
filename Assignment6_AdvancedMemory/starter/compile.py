import os

# Compile all of our tests
os.system('clang tests/test1.c -o tests/test1')
os.system('clang tests/test2.c -o tests/test2')
os.system('clang tests/test3.c -o tests/test3')
os.system('clang tests/t_test.c -o tests/test4')
os.system('clang tests/t_test2.c -o tests/test5')
os.system('clang tests/test6.c -o tests/test6')
os.system('clang tests/simpletest.c -o tests/simpletest')

# (Optional)
# Make sure my tests do not have memory leaks
# Valgrind also has helpful information
# about how many allocs and frees took place
os.system('valgrind ./tests/test1')
os.system('valgrind ./tests/test2')
os.system('valgrind ./tests/test3')

# Compile our malloc program
os.system('clang -c mymalloc.c')
#os.system('clang -c mymalloc_test.c -o mymalloc2.o')

# Compile our tests with our custom allocator
os.system('clang -I. -o ./tests/test1_mymalloc ./tests/test1.c mymalloc.o')
os.system('clang -I. -o ./tests/test2_mymalloc ./tests/test2.c mymalloc.o')
os.system('clang -I. -o ./tests/test3_mymalloc ./tests/test3.c mymalloc.o')
os.system('clang -I. -o ./tests/test4_mymalloc ./tests/t_test.c mymalloc.o')
os.system('clang -I. -g  -lpthread  -o ./tests/test5_mymalloc ./tests/t_test2.c mymalloc.o')
os.system('clang -I. -lpthread -o ./tests/test6_mymalloc ./tests/test6.c mymalloc.o')
os.system('clang -I. -lpthread -o ./tests/simpletest_mymalloc ./tests/simpletest.c mymalloc.o')
#os.system('clang -I. -o ./tests/test4_mymalloc2 ./tests/t_test.c mymalloc2.o')
#os.system('clang -I. -g  -lpthread  -o ./tests/test5_mymalloc2 ./tests/t_test2.c mymalloc2.o')
#os.system('clang -I. -lpthread -o ./tests/test6_mymalloc2 ./tests/test6.c mymalloc2.o')
