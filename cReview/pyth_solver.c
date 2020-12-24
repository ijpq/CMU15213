#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <math.h>

int main(int argc, char *argv[]) {

	/*
     * What does char *otparg represent? What about int
     * optind, int opterr, and int optopt?
     * Reference the man page or ask for help if you are struggling
     * with reading the man page.
     */
    extern char *optarg;
    extern int optind, opterr, optopt;

    int errorFlag = !(argc == 7 || argc == 8);
    int negFlag = 0;

	//what is arg?
    char arg;

	//declaring verbose mode, a, b, c 
	int verbose = 0;
    (void)verbose;
    float a = -1;
    float b = -1;
    float c = -1;

    /*
     * What should opstring be, in order to parse arguments correctly?
     */
    char *optstring = "a:b:c:v";

    //TODO: what should be placed in the first two arguments, instead of 0 and NULL?
    while ((arg = getopt(argc, argv, optstring)) != -1) {
        switch (arg) {
            case 'v':
                //fill in
                verbose = 1;
                break;
            case 'a':
                //fill in
                if (*optarg != '-'){
                    a = atof(optarg);
                }
                else negFlag = 1;
                break;
            case 'b':
                //fill in
                
                if (*optarg != '-'){
                    b = atof(optarg);
                }
                else negFlag = 1;
                break;
            case 'c':
                //fill in
                if (*optarg != '-'){
                    c = atof(optarg);
                }
                else negFlag = 1;
                break;
            default:
                //fill in
                errorFlag = 1;
                break;
        }
    }

    //error checking. Edit to account for negative side values.
    if (errorFlag) {
        printf("Error: invalid arguments\n");
        exit(errorFlag);
    }

    else if (negFlag){
        printf("Error: negative argument\n");
        exit(negFlag);
    }

    else if (a != -1 && b != -1 && c != -1) {
        int match = (c == sqrt(a * a + b * b));
		
        //when should these be printed? hint: verbose mode
        if (verbose)
        {printf("a^2 = %f\n", a*a);
		printf("b^2 = %f\n", b*b);
		printf("c^2 = %f\n", c*c);
        }
        printf("Those values %s work\n", match ? "do" : "don't");
    }

    return 0;
}