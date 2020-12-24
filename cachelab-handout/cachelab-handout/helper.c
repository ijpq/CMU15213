#include<stdio.h>
#include<stdint.h>
#include "helper.h"


void testFunc(FILE *pFile){
    char inStruc;
    unsigned long address;
    int size;
    while(fscanf(pFile, " %c %lx,%d", &inStruc, &address, &size)>0){
        printf("parsing instruction:%c %lx %d\n", inStruc, address, size);

    }
    fclose(pFile);
    return;
    

}