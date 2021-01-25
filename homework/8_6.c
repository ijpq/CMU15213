#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    
    if (!argc) {
        return 0;
    }
    
    char *p = argv[0];
    int i = 0;
    int size = 0;
    printf("Command-line arguments\n");
    while (i != argc) {
        printf("argv[ %d ]: %s \n", i, p);
        while ((*p) != '\0') {
            p++;
        }
        p++;
        i++;
    }
    return 0;
}
