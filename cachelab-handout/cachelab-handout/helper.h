#ifndef HELP_H
#define HELP_H
#include<stdio.h>
#include<stdint.h>
typedef struct {
	int set;
	int block;
	int line;
	char *File;
	int verbose;
} addrStruct;

typedef struct {
	int hits;
	int misses;
	int evictions;
} resStruct;

typedef struct {
	int bit;
	int movbit;
} maskOption;

typedef struct {
	unsigned long timer;
	long tag; // tag in decimal.
	int valid; //valid bit
} unit;

void getoptHelper(int, char*[], addrStruct*);
void testFunc();
resStruct cacheSimulator(addrStruct*, resStruct);
unsigned long GetDecimalBit(unsigned long, maskOption);
	
void LoadCache(int, int, int, int, unit*, resStruct*, unsigned long*, char*);
void WriteCache(int, int, int, int, unit*, resStruct*, unsigned long*);


#endif
