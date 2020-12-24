#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    char *input = malloc(sizeof(char)*4); // Should allocate size 5 since scanf adds /0
    printf("Input your 4-digit Pin: ");
    scanf("%4s", input);
    if (strcmp("0123", input) == 0)
        printf("Access Granted\n");
    else
        printf("Access Denied\n");

    // Did not free allocated space
    return 0;
}
