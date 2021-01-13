#include <stdio.h>

int x_a;
void p1_a() {
    printf("p1_a: x_a = %d\n", x_a);
}

int x_b;
void p1_b() {
    printf("p1_b: x_b = %d\n", x_b);
}

int x_c;
int y_c;
void p1_c() {
    printf("p1_c: x_c = %d, y_c = %d\n", x_c, y_c);
}

int x_d = 7;
int y_d = 5;
void p1_d() {
    printf("p1_d: x_d = %d, y_d = %d\n", x_d, y_d);
}
