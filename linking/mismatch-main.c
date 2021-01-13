#include <stdio.h>

long int x;  /* Weak symbol */

int main(int argc, char *argv[]) {
    printf("%ld\n", x);
    return 0;
}
