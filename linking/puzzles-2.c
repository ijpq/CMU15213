#include <stdio.h>

void p2_a() {
    printf("Calling p2_a\n");
}

int x_b;
void p2_b() {
    printf("p2_b: Initial: x_b = %d\n", x_b);
    x_b = 20;
    printf("p2_b: Final:   x_b = %d\n", x_b);
}

double x_c;
void p2_c() {
    printf("p2_c: Initial: x_c = %f\n", x_c);
    x_c = 2.0e20;
    printf("p2_c: Final:   x_c = %f\n", x_c);
}

double x_d;
void p2_d() {
    printf("p2_d: Initial: x_d = %f\n", x_d);
    x_d = 2.1e21; 
    printf("p2_d: Final:   x_d = %f\n", x_d);
}

int x_e;
void p2_e() {
    printf("p2_e: x_e = %d\n", x_e);
}
