
EXTENSIONS
        .s: assembly code
        .d: dissassembled object file
        .o: relocatable object file
        .a: static library
        .so: shared library

FILES
Makefile
        "make clean; make" generates all files

main.c
sum.c
        Simple example program 
prog
        Executable for simple example program

symbols.c
	Demonstration of what constitutes a symbol in a program

static-local.c
	Demonstration of how static variables are handled

puzzles-1.c puzzles-2.c puzzles-main.c
	Demonstration of linking rules given as puzzles in lecture

mismatch-main.c mismatch-variable.c
	Demonstration of program error due to type mismatch with global variables

global-c1.c global-c2.c global.h
	Demonstration of use of extern specifier in .h file to avoid type mismatches

main2.c
addvec.c
multvec.c
vector.h
        Example program for linking at compile time and load time
prog2c
prog2l
        Executables for main2.c linked at compile and load time

dll.c
        Example program for dynamic linking at run time
prog2r
        Executable for dll.c linked at run time

interpose/
        Interpositioning case study
