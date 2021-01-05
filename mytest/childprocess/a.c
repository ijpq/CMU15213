#include <stdio.h>
#include "../csapp.h"
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (Fork() == 0) {
        printf("child process, pid:%d\n", getpid());
        Execve("/bin/date", argv, NULL);
    }
    printf("pid:%d\n", getpid());
    return 0;
}
