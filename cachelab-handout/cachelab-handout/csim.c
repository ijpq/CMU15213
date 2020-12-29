#include "cachelab.h"
#include "helper.c"

int main(int argc, char* argv[])
{
    
	//testFunc(argv[1]);
	addrStruct addr;
	addrStruct *pAddr = &addr;
	char File[100];
    getoptHelper(argc, argv, pAddr, File);
	resStruct OutputRes = {0, 0, 0};
	resStruct *pRes = &OutputRes;
	cacheSimulator(File, pAddr, pRes);	
    printSummary(pRes->hits, pRes->misses, pRes->evictions);
    return 0;
}
