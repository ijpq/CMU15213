#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void snooze(int s) {
    int ret;
    ret = sleep(s);
    printf("Slept %d of %d secs.\n", s-ret, s);
    return;
}

void sigint_handler(int t) {
    return ;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler);
    
    int secs = 0;
    secs = *argv[1] - '0';
    snooze(secs);
    return 0;
    
}
