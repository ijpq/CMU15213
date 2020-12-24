CC=gcc
CFLAGS=-Wall -Werror
CLIBS=-lm

all: getopt_example pyth_solver

getopt_example:
	$(CC) $(CFLAGS) getopt_example.c -o getopt_example

pyth_solver:
	$(CC) $(CFLAGS) pyth_solver.c $(CLIBS) -o pyth_solver
	chmod 755 run_tests

clean:
	rm -f getopt_example pyth_solver
