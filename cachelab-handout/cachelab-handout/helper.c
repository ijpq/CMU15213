#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "helper.h"
#include <getopt.h>

#define MAXRES 17
#define MAXTAGLEN 8
#define ADDRLEN 64
#define SCALE 2

void getoptHelper(int argc, char* argv[], addrStruct* addrArg){
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
				addrArg->File = optarg;
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

resStruct cacheSimulator(addrStruct* Arg, resStruct res){
	resStruct *pRes = &res;
	
	// fscanf vars
	char op;
	unsigned long addr;
	int bytes;

	int cacheSize = Arg->set * Arg->line * sizeof(unit);
	unit *pCache = malloc(cacheSize);
	//unit* pTarget = pCache;
	if (!pCache) exit(1);
	
	int block = 0, set = 0, tag = 0,
	   	line = Arg->line; // this var save result in decimal.

	int64_t maskCO, maskCI, maskCT; // address mask
	maskOption optionCO = { Arg->block, 0},\
			   optionCI = { Arg->set, Arg->block },\
			   optionCT = { ADDRLEN - Arg->block - Arg->set,\
							Arg->block + Arg->set };
	FILE* pFile = fopen(Arg->File,"r");
	unsigned long time = 0;
	unsigned long *ptime = &time;
	char resChar[MAXRES] = {};

	while (fscanf(pFile, " %c %lx,%d\n", &op, &addr, &bytes) != 3) {
		block = GetDecimalBit(addr, optionCO);
		set = GetDecimalBit(addr, optionCI);
		tag = GetDecimalBit(addr, optionCT);
		
		memset(resChar, 0, sizeof(resChar));
		switch (op) {
			case 'L': {
				char *tmpres = NULL;
				tmpres = LoadCache(tag, set, block, line, pCache, pRes, ptime,\
				 resChar);
				strcpy(resChar, tmpres);
				break;
			}
			case 'S': {
				char *tmpres = NULL;
				tmpres = LoadCache(tag, set, block, line, pCache, pRes, ptime,\
				 resChar);
				strcpy(resChar, tmpres);
				break;
			}
			case 'M': {
				char *tmpres = NULL;
				tmpres = LoadCache(tag, set, block, line, pCache, pRes, ptime,\
				 resChar);
				strcpy(resChar, tmpres);
				strcat(resChar, " hit");
				pRes->hits++;
				break;
			}
		}
		if (Arg->verbose) {
			printf("%c %lx,%d %s\n", op, addr, bytes, resChar);
		}

	}	
	fclose(pFile);
	free(pCache);
	return res;
}

char* LoadCache(int tag,
			   int set,
			   int block,
			   int line,
			   unit* pCache,
			   resStruct* pRes,
			   unsigned long *time,
			   char ret[MAXRES] ) {
	int get = 0;
	unit *pTarget = NULL;
	while (!get) {
		for (int l = 0; l < line; ++l) {
			pTarget = pCache + (set*line+l)*sizeof(unit);
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
			//(*time)++;
			WriteCache(tag, set, block, line, pCache, pRes, time);
			(pRes->misses)++;
			strcpy(ret, "miss eviction");
			get = 1;
			//return ret;
		}
	}
	++*time;
	return ret;

}

void WriteCache(int tag,
				int set,
			   	int block,
			   	int line,
			   	unit* pCache,
				resStruct *pRes,
			   	unsigned long *time) {
	// simulate behavior , write tag. 
	unit *pBlock = NULL;
	long int LeastRecentUsed = INT64_MAX, LRUIdx = 0;

	// prior to write empty block.
	for (int l = 0; l < line; l++) {
		pBlock = pCache + (set*line+l)*sizeof(unit);
		if (pBlock->timer < LeastRecentUsed) {
			LeastRecentUsed = pBlock->timer;
			LRUIdx = l;
		}
	}
	pBlock = pCache + (set*line+LRUIdx)*sizeof(unit);
	pBlock->tag = tag;
	pBlock->timer = *time;
	(pRes->evictions)++;
	++*time;
}
unsigned long  GetDecimalBit(unsigned long addr, maskOption opt) {
	int64_t mask = (1 << opt.bit) - 1;
	mask <<= opt.movbit;
	addr &= mask;
	addr >>= opt.movbit;
	return addr;
}
	

void testFunc(const char* FilePath){
    FILE* pFile = fopen(FilePath, "r");
    char inStruc;
    unsigned long address;
    int size;
    while(fscanf(pFile, " %c %lx,%d", &inStruc, &address, &size)>0){
        printf("parsing instruction:%c %lx %d\n", inStruc, address, size);

    }
    fclose(pFile);
    return;
    

}

//		int main(){
//				return 0;
//		}
