#include "cachelab.h"
#include "helper.c"

int main(int argc, char* argv[])
{
    
    retStruct ret = getoptHelper(argc, argv);
    
    

    //testFunc(fopen(argv[1],"r"));
    printSummary(0, 0, 0);
    return 0;
}
