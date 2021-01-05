
#include "../csapp.h"
#include <signal.h>
#include <stdio.h>

void sigint_handler(int sig) {
    printf("\n caught SIGINT, this message printed within handler func\n");
    exit(0);
}

void unix_error(char *str) {
    printf("%s\n",str);
}

int main(void) {
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        unix_error("signal error\n");
    }

    pause();
    return 0;
}
