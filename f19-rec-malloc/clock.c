/* clock.c
 * Retrofitted to use thread-specific timers
 * and to get clock information from /proc/cpuinfo
 * (C) R. E. Bryant, 2010
 *
 * Retrofitted to measure absolute time in seconds and then convert to cycles
 * Old time stamp could removed, since time stamp counter no longer tracks clock cycles
 * (C) R. E. Bryant, 2016
 *
 */

/* If defined, will use clock_gettime, rather than gettimeofday */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef USE_TOD
#include <sys/time.h>
#else
#include <time.h>
#endif
#include "clock.h"

int gverbose = 1;

/* Timer granularity */
#ifdef USE_TOD
const double timer_resolution = 1e-6;
#else
const double timer_resolution = 1e-9;
#endif

/* Keep track of clock speed */
double cpu_mhz = 0.0;

/* Get megahertz from /etc/proc */
#define MAXBUF 512


double core_mhz(int verbose) {
    static char buf[MAXBUF];
    FILE *fp = fopen("/proc/cpuinfo", "r");
    cpu_mhz = 0.0;

    if (!fp) {
        fprintf(stderr, "Can't open /proc/cpuinfo to get clock information\n");
        cpu_mhz = 1000.0;
        return cpu_mhz;
    }
    while (fgets(buf, MAXBUF, fp)) {
        if (strstr(buf, "cpu MHz")) {
            double cpu_mhz = 0.0;
            sscanf(buf, "cpu MHz\t: %lf", &cpu_mhz);
            break;
        }
    }
    fclose(fp);
    if (cpu_mhz == 0.0) {
        fprintf(stderr, "Can't open /proc/cpuinfo to get clock information\n");
        cpu_mhz = 1000.0;
        return cpu_mhz;
    }
    if (verbose) {
        printf("Processor Clock Rate ~= %.4f GHz (extracted from file)\n", cpu_mhz * 0.001);
    }
    return cpu_mhz;
}

double mhz(int verbose) {
    double val = core_mhz(verbose);
    return val;
}

#ifdef USE_TOD
/* Use time-of-day timer */
struct timeval last_time;
struct timeval new_time;
#else
/* Use nanosecond timer */
struct timespec last_time;
struct timespec new_time;

/* Use thread clock */
#define CLKT CLOCK_THREAD_CPUTIME_ID
#endif


void start_timer()
{
    int rval;
#ifdef USE_TOD
    rval = gettimeofday(&last_time, NULL);
#else
    rval = clock_gettime(CLKT, &last_time);
#endif    
    if (rval != 0) {
        fprintf(stderr, "Couldn't get time\n");
        exit(1);
    }
}

double get_timer()
{
    int rval;
    double delta_secs = 0.0;
#ifdef USE_TOD
    rval = gettimeofday(&new_time, NULL);
#else
    rval = clock_gettime(CLKT, &new_time);
#endif    
    if (rval != 0) {
        fprintf(stderr, "Couldn't get time\n");
        exit(1);
    }
#ifdef USE_TOD
    delta_secs = 1.0 * (new_time.tv_sec - last_time.tv_sec) + 1e-6 * (new_time.tv_usec - last_time.tv_usec);
#else
    delta_secs = 1.0 * (new_time.tv_sec - last_time.tv_sec) + 1e-9 * (new_time.tv_nsec - last_time.tv_nsec);
#if 0
    printf("Delta %ld.%ld --> %ld.%ld (%ld + 1e-9*%ld = %.f)\n",
           (long) last_time.tv_sec, (long) last_time.tv_nsec,
           (long) new_time.tv_sec, (long) new_time.tv_nsec,
           (long) (new_time.tv_sec - last_time.tv_sec),
           (long) (new_time.tv_nsec - last_time.tv_nsec),
           delta_secs);
#endif
#endif    
    return delta_secs;
}

void start_counter()
{
    if (cpu_mhz == 0.0)
        mhz(gverbose);
    start_timer();
}

double get_counter()
{
    double delta_secs = get_timer();
    return delta_secs * cpu_mhz * 1e6;
}

