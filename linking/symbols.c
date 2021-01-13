#include <stdio.h>

int incr = 1;

static int foo(int a) {
    int b = a + incr;
    return b;
}

int main(int argc, char *argv[])
{
    printf("%d\n", foo(5));
    return 0;
}
