#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "helper.h"
#include <getopt.h>

#define MAXRES 100
#define MAXTAGLEN 8
#define ADDRLEN 64
#define SCALE 2

void getoptHelper(int argc, char* argv[], addrStruct* addrArg, char File[]){
    extern char* optarg;
    extern int optind, opterr, optopt;
    
    int errorFlag = !(argc >= 9 && argc <= 11);
    
    char* optstring = "hvs:E:b:t:";
	char arg;

    while ((arg = getopt(argc, argv, optstring)) != -1){
        switch(arg){
            case 'h':
                //usage = 1;
                printf("USAGE INFO:\n");
                break;
            case 'v':
				addrArg->verbose = 1;
                break;
            case 's':
                addrArg->set = atoi(optarg);
				break;
			case 'E':
				addrArg->line = atoi(optarg);
				break;
			case 'b':
				addrArg->block = atoi(optarg);
				break;
			case 't':
				//File = optarg;
				strcpy(File, optarg);
				break;
			default:
				errorFlag = 1;
				break;
        }
    }
	if (errorFlag) {
		printf("invalid argument!\n");
		exit(errorFlag);
	}
	else if(addrArg->verbose) {
		//printf("verbose mode\n");
	}
	//printf("parsing arg done\n");

}

void cacheSimulator(char File[], addrStruct* Arg, resStruct *pRes){
	
	// fscanf vars
	char op;
	unsigned long addr;
	int bytes;

	int numOfSet = (int) pow(2,Arg->set);
	int numOfLine = Arg->line;
	long long cacheSize = numOfLine * numOfSet * sizeof(unit);
	unit *pCache = (unit*)malloc(cacheSize);
	memset(pCache, 0, cacheSize);
	if (!pCache) exit(1);
	
	int block = 0, set = 0, tag = 0,
	   	line = Arg->line; // this var save result in decimal.

	maskOption optionCO = { Arg->block, 0},\
			   optionCI = { Arg->set, Arg->block },\
			   optionCT = { ADDRLEN - Arg->block - Arg->set,\
							Arg->block + Arg->set };
	unsigned long time = 0;
	unsigned long *ptime = &time;
	char resChar[MAXRES] = {};
	FILE* pFile = fopen(File,"r");

	while (fscanf(pFile, " %c %lx,%d", &op, &addr, &bytes) == 3) {
		block = GetDecimalBit(addr, optionCO);
		set = GetDecimalBit(addr, optionCI);
		tag = GetDecimalBit(addr, optionCT);
		
		memset(resChar, 0, sizeof(resChar));
		if (op == 'M') {
			LoadCache(tag, set, block, line, pCache, pRes, ptime, resChar);
			strcat(resChar, " hit");
			pRes->hits++;
		} else if (op == 'L' || op == 'S') {
			LoadCache(tag, set, block, line, pCache, pRes, ptime, resChar);
		  }

		if (Arg->verbose) {
			printf("%c %lx,%d %s\n", op, addr, bytes, resChar);
		}
	}	
	free(pCache);
	fclose(pFile);

}

void LoadCache(int tag,
			   const int set,
			   int block,
			   const int line,
			   unit* pCache,
			   resStruct* pRes,
			   unsigned long *time,
			   char ret[] ) {
	int get = 0;
	unit *pTarget = NULL;
	while (!get) {
		for (int l = 0; l < line; ++l) {
			pTarget = pCache + (set*line+l);
			if (pTarget->valid && pTarget->tag == tag){
				(pRes->hits)++;
				pTarget->timer = *time;
				get = 1;
				strcpy(ret, "hit");
				break;
			} else if (!pTarget->valid) {
				(pRes->misses)++;
				pTarget->timer = *time;
				pTarget->valid = 1;
				pTarget->tag = tag;
				get = 1;
				strcpy(ret, "miss");
				break;
			}
		}
		
		if (!get) {
			WriteCache(tag, set, block, line, pTarget, pRes, time);
			(pRes->misses)++;
			strcpy(ret, "miss eviction");
			get = 1;
			break;
		}
	}
	(*time)++;

}

void WriteCache(const int tag,
				const int set,
			   	const int block,
			   	const int line,
			   	unit* pTarget,
				resStruct *pRes,
			   	unsigned long *time) {
	// simulate behavior , write tag. 
	unit *pBlock = NULL;
	unsigned long LeastRecentUsed = 999999;
    int	LRUIdx = 0;
	
	// prior to write empty block.
	for (int l = 0; l < line; l++) {
		pBlock = pTarget - l;
		if ((pBlock->timer) < LeastRecentUsed) {
			LeastRecentUsed = pBlock->timer;
			LRUIdx = l;
		}
	}
	unit* pNewTarget = pTarget - LRUIdx;
	pNewTarget->tag = tag;
	pNewTarget->timer = *time;
	(pRes->evictions)++;
	(*time)++;
}
unsigned long  GetDecimalBit(unsigned long addr, maskOption opt) {
	int64_t mask = (1 << opt.bit) - 1;
	mask <<= opt.movbit;
	addr &= mask;
	addr >>= opt.movbit;
	return addr;
}
	

void InitCache(unit* pCache, int numOfBlock) {
	for (int i =0; i<numOfBlock; ++i) {
		(pCache+i)->valid = 0;
		(pCache+i)->tag = 0;
		(pCache+i)->timer = 0;
	}
}
    

