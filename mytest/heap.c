
// this file tests returning value of `malloc` library in c library.
// testing process should be performed by GDB

#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int *startPtr = (int*) malloc(2*sizeof(int));
    *startPtr = 7;
    int *secondPtr = startPtr+1;
    *secondPtr = 5;
    printf("after assignment, value and address is as follow:\n");
    printf("first element - addr:%p,\tvalue:%d\n",startPtr, *startPtr);
    printf("second element - addr:%p,\tvalue:%d\n",secondPtr, *secondPtr);
    return 0;

    

}
