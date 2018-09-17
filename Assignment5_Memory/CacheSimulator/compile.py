import os

os.system("gcc -std=c11 -lm trace.c -o trace")
os.system("gcc -std=c11 -lm simulator_fifo.c -o simulator")
