#include <stdio.h>

int x_a;
void p1_a() {
    printf("p1_a Initial: x_a = %d\n", x_a);
    x_a = 10;
    printf("p1_a Final:   x_a = %d\n", x_a);
}

int x_b;
void p1_b() {
    printf("p1_b: Initial: x_b = %d\n", x_b);
    x_b = 11;
    printf("p1_b: Final:   x_b = %d\n", x_b);
}

int y_c;
int x_c;
void p1_c() {
    printf("p1_c: Initial: x_c = %d, y_c = %d\n", x_c, y_c);
    x_c = 12; y_c = 13;
    printf("p1_c: Final:   x_c = %d, y_c = %d\n", x_c, y_c);
}

int y_d = 5;
int x_d = 7;
void p1_d() {
    printf("p1_d: x_d = %d, y_d = %d\n", x_d, y_d);
}

int x_e = 7;
void p1_e() {
    printf("p1_e: x_e = %d\n", x_e);
}

