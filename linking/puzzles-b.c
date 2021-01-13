#include <stdio.h>

void p2_a() {
    printf("Calling p2_a\n");
}

int x_b;
void p2_b() {
    printf("p2_b: x_b = %d\n", x_b);
}

double x_c;
void p2_c() {
    x_c = 3.14;
    printf("p2_c: x_c = %f\n", x_c);
}

double x_d;
void p2_d() {
    x_d = 3.14; 
    printf("p2_d: x_d = %f", x_d);
}
