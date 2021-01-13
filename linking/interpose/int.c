/* 
 * int.c - Example program to demonstrate different ways to
 *           interpose on the malloc and free functions.
 *
 * Note: be sure to compile unoptimized (-O0) so that gcc won't
 * optimize away the calls to malloc and free.
 */
/* $begin interposemain */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int i;
    for (i = 1; i < argc; i++) {
	void *p = malloc(atoi(argv[i]));
	free(p);
    }
    return(0); 
}
/* $end interposemain */
