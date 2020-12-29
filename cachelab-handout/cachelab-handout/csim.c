#include "cachelab.h"
#include "helper.c"

int main(int argc, char* argv[])
{
    
	//testFunc(argv[1]);
	addrStruct addr;
	addrStruct *pAddr = &addr;
    getoptHelper(argc, argv, pAddr);
	resStruct OutputRes = {0, 0, 0};
	resStruct res = cacheSimulator(pAddr, OutputRes);	

    printSummary(res.hits, res.misses, res.evictions);
    return 0;
}
