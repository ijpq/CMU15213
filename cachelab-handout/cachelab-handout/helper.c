#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "helper.h"
#include <getopt.h>

retStruct getoptHelper(int argc, char* argv[]){
    extern char* optarg;
    extern int optind, opterr, optopt;
    
    int errorFlag = !(argc >= 9 && argc <= 11);
    
    // option
    char arg;
    
    int verbose = 0, usage = 0;
	int block, line, set;
    char* traceFile;


    char* optstring = "hvs:E:b:t:";

    while ((arg = getopt(argc, argv, optstring)) != -1){
        switch(arg){
            case 'h':
                usage = 1;
                printf("USAGE INFO:\n");
                break;
            case 'v':
                verbose = 1;
                break;
            case 's':
                set = atoi(optarg);
				break;
			case 'E':
				line = atoi(optarg);
				break;
			case 'b':
				block = atoi(optarg);
				break;
			case 't':
				traceFile = optarg;
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
	else {}
	retStruct ret = {set, block, line, traceFile};
    return ret;
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
