#include <stdio.h>

void p1_a();
void p1_b();
void p1_c();
void p1_d();
void p1_e();

void p2_a();
void p2_b();
void p2_c();
void p2_d();
void p2_e();

int main(int argc, char *argv[]) {
    char choice = 'a';
    if (argc >= 2) {
	choice = argv[1][0];
    }
    switch (choice) {
    case 'a':
	p1_a();
	p2_a();

	p2_a();
	p1_a();
	break;
    case 'b':
	p1_b();
	p2_b();

	p2_b();
	p1_b();
	break;
    case 'c':
	p1_c();
	p2_c();
	p1_c();
	p2_c();
	break;
    case 'd':
	p1_d();
	p2_d();
	p1_d();
	break;
    case 'e':
	p1_e();
	p2_e();
	break;
    default:
	printf("Unknown choice '%c'\n", choice);
	
    }
    return 0;
}
