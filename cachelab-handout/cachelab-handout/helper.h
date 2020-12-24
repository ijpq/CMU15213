#ifndef HELP_H
#define HELP_H
#include<stdio.h>
typedef struct retStruct {
	int set;
	int block;
	int line;
	char *File;
} retStruct;
retStruct getoptHelper(int, char*[]);
void testFunc();

#endif
