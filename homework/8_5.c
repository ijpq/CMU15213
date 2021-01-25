#include <unistd.h>
unsigned int snooze (unsigned int secs);


unsigned int snooze (unsigned int secs) {
    unsigned int ret;

    ret = sleep(secs);
    printf("Slept for %u of %u secs.\n", secs-ret, secs);
    return ret;
}
