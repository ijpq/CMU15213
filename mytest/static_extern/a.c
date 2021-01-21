
#include "a.h"
#include <stdio.h>

int main(void) {
    extern int a;
    extern int b;
    printf("%d\n",a);
    printf("%d\n",b);
    
    return 0;
}
