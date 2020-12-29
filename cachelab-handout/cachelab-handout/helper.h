#ifndef HELP_H
#define HELP_H
#include<stdio.h>
#include<stdint.h>
typedef struct {
	int set;
	int block;
	int line;
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

void getoptHelper(int, char*[], addrStruct*, char []);
unsigned long GetDecimalBit(unsigned long, maskOption);
	
void cacheSimulator(char [],addrStruct*, resStruct*);
void LoadCache(int, const int, int, const int, unit*, resStruct*, unsigned long*, char*);
void WriteCache(const int, const int, const int, const int, unit*, resStruct*, unsigned long*);
void InitCache(unit*, int);


#endif
