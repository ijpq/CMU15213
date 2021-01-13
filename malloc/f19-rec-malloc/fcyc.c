/* Compute time used by function f */
#include <stdlib.h>
#include <sys/times.h>
#include <stdio.h>

#include "clock.h"
#include "fcyc.h"

#define K 3
#define MAXSAMPLES 20
#define EPSILON 0.01 
#define CLEAR_CACHE 0
#define CACHE_BYTES (1<<19)
#define CACHE_BLOCK 32
#define MIN_TICKS 1000
#define MIN_REPS 8

static long int kbest = K;
static int clear_cache = CLEAR_CACHE;
static long int maxsamples = MAXSAMPLES;
static double epsilon = EPSILON;
static long int cache_bytes = CACHE_BYTES;
static long int cache_block = CACHE_BLOCK;
static long int min_reps = MIN_REPS;
static long int min_ticks = MIN_TICKS;
static double min_time = 0;

static long int *cache_buf = NULL;

static double *values = NULL;
static long int samplecount = 0;

#define KEEP_VALS 0
#define KEEP_SAMPLES 0

#if KEEP_SAMPLES
static double *samples = NULL;
#endif

/* Initialize the minimum time threshold */
static void init_min_time() {
    if (min_time == 0.0)
        min_time = min_ticks * timer_resolution;
}

/* Start new sampling process */
static void init_sampler()
{
    if (values)
        free(values);
    values = calloc(kbest, sizeof(double));
#if KEEP_SAMPLES
    if (samples)
        free(samples);
    /* Allocate extra for wraparound analysis */
    samples = calloc(maxsamples+kbest, sizeof(double));
#endif
    samplecount = 0;
}

/* Add new sample.  */
static void add_sample(double val)
{
    long int pos = 0;
    if (samplecount < kbest) {
        pos = samplecount;
        values[pos] = val;
    } else if (val < values[kbest-1]) {
        pos = kbest-1;
        values[pos] = val;
    }
#if KEEP_SAMPLES
    samples[samplecount] = val;
#endif
    samplecount++;
    /* Insertion sort */
    while (pos > 0 && values[pos-1] > values[pos]) {
        double temp = values[pos-1];
        values[pos-1] = values[pos];
        values[pos] = temp;
        pos--;
    }
}

/* Have kbest minimum measurements converged within epsilon? */
static long int has_converged()
{
    return
        (samplecount >= kbest) &&
        ((1 + epsilon)*values[0] >= values[kbest-1]);
}

/* Code to clear cache */


static volatile long int sink = 0;

static void clear()
{
    long int x = sink;
    long int *cptr, *cend;
    long int incr = cache_block/sizeof(long int);
    if (!cache_buf) {
        cache_buf = malloc(cache_bytes);
        if (!cache_buf) {
            fprintf(stderr, "Fatal error.  Malloc returned null when trying to clear cache\n");
            exit(1);
        }
    }
    cptr = (long int *) cache_buf;
    cend = cptr + cache_bytes/sizeof(long int);
    while (cptr < cend) {
        x += *cptr;
        cptr += incr;
    }
    sink = x;
}

double fcyc(test_funct f, void *args)
{
    double result;
    long reps = min_reps;
    long r;
    double cyc;
    /* Increase reps until get meaningful times */
    double sec = 0.0;
    init_min_time();
    while (sec < min_time) {
        if (clear_cache)
            clear();
        start_timer();
        for (r = 0; r < reps; r++) {
            f(args);
        }
        sec = get_timer();
        if (sec < min_time)
            reps += reps;
    }
    init_sampler();
    do {
        if (clear_cache)
            clear();
        start_counter();
        for (r = 0; r < reps; r++) {
            f(args);
        }
        cyc = (double) get_counter() / reps;
        if (cyc > 0.0)
            add_sample(cyc);
    } while (!has_converged() && samplecount < maxsamples);
    result = values[0];
#if !KEEP_VALS
    free(values); 
    values = NULL;
#endif
    return result;  
}

double fsec(test_funct f, void *args)
{
    double result;
    /* Increase reps until get meaningful times */
    long reps = min_reps;
    long r;
    double sec = 0.0;
    init_min_time();
    while (sec < min_time) {
        if (clear_cache)
            clear();
        start_timer();
        for (r = 0; r < reps; r++) {
            f(args);
        }
        sec = get_timer();
        if (sec < min_time)
            reps += reps;
        //        printf("uSecs = %.3f, reps = %ld\n", sec * 1e6, reps);
    }
    init_sampler();
    //    printf("\nuSecs (reps=%ld):", reps);
    do {
        if (clear_cache)
            clear();
        start_timer();
        for (r = 0; r < reps; r++) {
            f(args);
        }
        sec = get_timer()/reps;
        //        printf(" %.3f", sec * 1e6);
        if (sec > 0.0)
            add_sample(sec);
    } while (!has_converged() && samplecount < maxsamples);
    result = values[0];
    //    printf(" --> %.3f\n", result * 1e6);
#if !KEEP_VALS
    free(values); 
    values = NULL;
#endif
    return result;  
}


/***********************************************************/
/* Set the various parameters used by measurement routines */


/* Sets minimum number of timer ticks to resolve time.  Default = 100 */
void set_fcyc_min_ticks(int t) {
    min_ticks = t;
}

/* Sets minimum number of repetitions of function.  Default = 8 */
void set_fcyc_min_reps(int r) {
    min_reps = r;
}

/* When set, will run code to clear cache before each measurement 
   Default = 0
*/
void set_fcyc_clear_cache(int clear)
{
    clear_cache = clear;
}

/* Set size of cache to use when clearing cache 
   Default = 1<<19 (512KB)
*/
void set_fcyc_cache_size(long int bytes)
{
    if (bytes != cache_bytes) {
        cache_bytes = bytes;
        if (cache_buf) {
            free(cache_buf);
            cache_buf = NULL;
        }
    }
}

/* Set size of cache block 
   Default = 32
*/
void set_fcyc_cache_block(long int bytes) {
    cache_block = bytes;
}

/* Value of K in K-best
   Default = 3
*/
void set_fcyc_k(long int k)
{
    kbest = k;
}

/* Maximum number of samples attempting to find K-best within some tolerance.
   When exceeded, just return best sample found.
   Default = 20
*/
void set_fcyc_maxsamples(long int maxsamples_arg)
{
    maxsamples = maxsamples_arg;
}

/* Tolerance required for K-best
   Default = 0.01
*/
void set_fcyc_epsilon(double epsilon_arg)
{
    epsilon = epsilon_arg;
}





