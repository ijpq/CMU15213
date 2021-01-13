#include <stdio.h>
#include <stdlib.h>
#include "global.h"

int g = 0;

int main(int argc, char *argv[]) {
    if (argc >= 2) {
	g = atoi(argv[1]);
    }
    printf("g = %d.  f() = %d\n", g, f());
    return 0;
}

