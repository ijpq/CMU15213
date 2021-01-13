/*
 * Mdriver.c - Autolab version of the CS:APP Malloc Lab Driver
 *
 * Uses a collection of trace files to tests a malloc/free
 * implementation in mm.c.
 *
 * Copyright (c) 2004-2016, R. Bryant and D. O'Hallaron, All rights
 * reserved.  May not be used, modified, or copied without permission.
 */
#include <assert.h>
#include <errno.h>
#include <float.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

#include "mm.h"
#include "memlib.h"
#include "fcyc.h"
#include "config.h"
#include "stree.h"

/**********************
 * Constants and macros
 **********************/

/* Misc */
#define MAXLINE     1024          /* max string size */
#define HDRLINES       4          /* number of header lines in a trace file */
#define LINENUM(i) (i+HDRLINES+1) /* cnvt trace request nums to linenums (origin 1) */

#ifndef REF_ONLY
#define REF_ONLY 0
#endif

/* Returns true if p is ALIGNMENT-byte aligned */
#define IS_ALIGNED(p)  ((((unsigned long)(p)) % ALIGNMENT) == 0)

/* weights */
typedef enum { WNONE, WALL, WUTIL, WPERF } weight_t;

/******************************
 * The key compound data types
 *****************************/

/*
 * There are two different, easily-confusable concepts:
 * - opnum: which line in the file.
 * - index: the block number ; corresponds to something allocated.
 * Remember that index (-1) is the null pointer.
 */

/*
 * Records the extent of each block's payload.
 * Organized as doubly linked list
 */
typedef struct range_t {
    char *lo;              /* low payload address */
    char *hi;              /* high payload address */
    long index;            /* same index as free; for debugging */
    struct range_t *next;
    struct range_t *prev;
} range_t;

/*
 * All information about set of ranges represented as doubly-linked
 * list of ranges, plus a splay tree keyed by lo addresses
 */
typedef struct {
    range_t *list;
    tree_t *lo_tree;
} range_set_t;

/* Characterizes a single trace operation (allocator request) */
typedef struct {
    enum { ALLOC, FREE, REALLOC } type; /* type of request */
    long index;                         /* index for free() to use later */
    size_t size;                        /* byte size of alloc/realloc request */
} traceop_t;

/* Holds the information for one trace file */
typedef struct {
    char filename[MAXLINE];
    size_t data_bytes;    /* Peak number of data bytes allocated during trace */
    int num_ids;          /* number of alloc/realloc ids */
    int num_ops;          /* number of distinct requests */
    weight_t weight;      /* weight for this trace */
    traceop_t *ops;       /* array of requests */
    char **blocks;        /* array of ptrs returned by malloc/realloc... */
    size_t *block_sizes;  /* ... and a corresponding array of payload sizes */
    int *block_rand_base; /* index into random_data, if debug is on */
} trace_t;

/*
 * Holds the params to the xxx_speed functions, which are timed by fcyc.
 * This struct is necessary because fcyc accepts only a pointer array
 * as input.
 */
typedef struct {
    trace_t *trace;
    range_set_t *ranges;
} speed_t;

/* Summarizes the important stats for some malloc function on some trace */
typedef struct {
    /* set in read_trace */
    char     filename[MAXLINE];
    weight_t weight;
    double   ops;      /* number of ops (malloc/free/realloc) in the trace */

    /* run-time stats defined for both libc and student */
    bool valid;        /* was the trace processed correctly by the allocator? */
    double secs;       /* number of secs needed to run the trace */
    double tput;       /* throughput for this trace in Kops/s */

    /* defined only for the student malloc package */
    double util;       /* space utilization for this trace (always 0 for libc) */

    /* Note: secs and util are only defined if valid is true */
} stats_t;

/* Summarizes the key statistics for a set of traces */
typedef struct {
    double util;  /* average utilization expressed as a percentage */
    double ops;   /* total number of operations */
    double secs;  /* total number of elapsed seconds */
    double tput;  /* average throughput expressed in Kops/s */
} sum_stats_t;

/********************
 * For debugging.  If debug-mode is on, then we have each block start
 * at a "random" place (a hash of the index), and copy random data
 * into it.  With DBG_CHEAP, we check that the data survived when we
 * realloc and when we free.  With DBG_EXPENSIVE, we check every block
 * every operation.
 * randint_t should be a byte, in case students return unaligned memory.
 *******************/
#define RANDOM_DATA_LEN (1<<16)

typedef unsigned char randint_t;
static const char randint_t_name[] = "byte";
static randint_t random_data[RANDOM_DATA_LEN];


/********************
 * Global variables
 *******************/

/* Global values */
typedef enum { DBG_NONE, DBG_CHEAP, DBG_EXPENSIVE } debug_mode_t;

static debug_mode_t debug_mode = REF_ONLY ? DBG_NONE : DBG_CHEAP;
int verbose = REF_ONLY ? 0 : 1;  /* global flag for verbose output */
static int errors = 0;           /* number of errs found when running student malloc */
static bool onetime_flag = false;
static bool tab_mode = false;     /* Print output as tab-separated fields */
/* If set, use sparse memory emulation */
static bool sparse_mode = SPARSE_MODE;
static size_t maxfill = SPARSE_MODE ? MAXFILL_SPARSE : MAXFILL;

/* by default, no timeouts */
static int set_timeout = 0;

/* Directory where default tracefiles are found */
static char tracedir[MAXLINE] = TRACEDIR;

/* The following are null-terminated lists of tracefiles that may or may not get used */

/* The filenames of the default tracefiles */
static char *default_tracefiles[] = {
    DEFAULT_TRACEFILES, NULL
};

/* The filenames of the default tracefiles that require sparse emulation */
static char *default_giant_tracefiles[] = {
    DEFAULT_GIANT_TRACEFILES, NULL
};

/* Store names of trace files as array of char *'s */
static int num_global_tracefiles = 0;
static char **global_tracefiles = NULL;

static char autoresult[MAXLINE]; /* autoresult string */

/* Summary statistics for libc and student's mm.c submissions */
static sum_stats_t global_libc_sum_stats;
static sum_stats_t global_mm_sum_stats;

/* Performance statistics for driver */

/*********************
 * Function prototypes
 *********************/

/* This function enables generating the set of trace files */
static void add_tracefile(char *trace);

/* these functions manipulate range sets */
static range_set_t *new_range_set();
static bool add_range(range_set_t *ranges, char *lo, size_t size,
                      const trace_t *trace, int opnum, int index);
static void remove_range(range_set_t *ranges, char *lo);
static void free_range_set(range_set_t *ranges);

/* These functions implement the debugging code */
static void init_random_data(void);
static bool check_index(const trace_t *trace, int opnum, int index);
static void randomize_block(trace_t *trace, int index);

/* These functions read, allocate, and free storage for traces */
static trace_t *read_trace(stats_t *stats, const char *tracedir,
                           const char *filename);
static void reinit_trace(trace_t *trace);
static void free_trace(trace_t *trace);

/* Routines for evaluating the correctness and speed of libc malloc */
static bool eval_libc_valid(trace_t *trace);
static void eval_libc_speed(void *ptr);

/* Routines for evaluating correctnes, space utilization, and speed
   of the student's malloc package in mm.c */
static bool eval_mm_valid(trace_t *trace, range_set_t *ranges);
static double eval_mm_util(trace_t *trace, int tracenum);
static void eval_mm_speed(void *ptr);

/* Various helper routines */
static void printresults(int n, stats_t *stats, sum_stats_t *sumstats);
static void usage(char *prog);
static void malloc_error(const trace_t *trace, int opnum, const char *fmt, ...)
    __attribute__((format(printf, 3,4)));
static void unix_error(const char *fmt, ...)
    __attribute__((format(printf, 1,2), noreturn));
static void app_error(const char *fmt, ...)
    __attribute__((format(printf, 1,2), noreturn));
static double compute_scaled_score(double value, double min, double max);

static sigjmp_buf timeout_jmpbuf;

/* Timeout signal handler */
static void timeout_handler(int sig __attribute__((unused))) {
    fprintf(stderr, "The driver timed out after %d secs\n", set_timeout);
    errors = 1;
    longjmp(timeout_jmpbuf, 1);
}

/* Compute throughput from reference implementation */
static double lookup_ref_throughput(bool checkpoint);
static double measure_ref_throughput(bool checkpoint);

/*
 * Run the tests; return the number of tests run (may be less than
 * num_tracefiles, if there's a timeout)
 */
static void run_tests(int num_tracefiles, const char *tracedir,
                      char **tracefiles,
                      stats_t *mm_stats, speed_t *speed_params) {
    volatile int i;

    for (i=0; i < num_tracefiles; i++) {
        /* initialize simulated memory system in memlib.c *
         * start each trace with a clean system */
        mem_init(sparse_mode);
        range_set_t *ranges = new_range_set();


        // NOTE: If times out, then it will reread the trace file

        trace_t *trace;
        trace = read_trace(&mm_stats[i], tracedir, tracefiles[i]);
        strcpy(mm_stats[i].filename, trace->filename);
        mm_stats[i].ops = trace->num_ops;

        /* Prepare for timeout */
        if (setjmp(timeout_jmpbuf) != 0) {
            mm_stats[i].valid = false;
        } else {
            if (verbose > 1)
                printf("Checking mm_malloc for correctness, ");
            mm_stats[i].valid =
                /* Do 2 tests, since may fail to reinitialize properly */
                eval_mm_valid(trace, ranges) && eval_mm_valid(trace, ranges);

            if (onetime_flag) {
                free_trace(trace);
                return;
            }
        }
        if (mm_stats[i].valid) {
            if (verbose > 1)
                printf("efficiency, ");
            mm_stats[i].util = eval_mm_util(trace, i);
            speed_params->trace = trace;
            speed_params->ranges = ranges;
            if (verbose > 1)
                printf("and performance.\n");
            mm_stats[i].secs = sparse_mode ? 1.0 : fsec(eval_mm_speed, speed_params);
            mm_stats[i].tput = mm_stats[i].ops / (mm_stats[i].secs * 1000.0);
        }

#if 0
        printf(" %d operations.  %ld comparisons.  Avg = %.1f\n",
               trace->num_ops, ranges->lo_tree->comparison_count,
               (double) ranges->lo_tree->comparison_count / trace->num_ops);
#endif
        free_trace(trace);
        free_range_set(ranges);

        /* clean up memory system */
        mem_deinit();
    }
}

/**************
 * Main routine
 **************/
int main(int argc, char **argv)
{
    int i;
    global_tracefiles = NULL;  /* array of trace file names */
    num_global_tracefiles = 0;    /* the number of traces in that array */

    stats_t *libc_stats = NULL;/* libc stats for each trace */
    stats_t *mm_stats = NULL;  /* mm (i.e. student) stats for each trace */
    speed_t speed_params;      /* input parameters to the xx_speed routines */

    bool run_libc = false;     /* If set, run libc malloc (set by -l) */
    bool autograder = false;   /* if set then called by autograder (-A) */
    bool checkpoint = false;

    setbuf(stdout, 0);
    setbuf(stderr, 0);

    double min_throughput = -1;
    double max_throughput = -1;


#if !REF_ONLY

    char c;
    /*
     * Read and interpret the command line arguments
     */
    while ((c = getopt(argc, argv, "d:f:c:s:t:v:hpOVAlDT")) != EOF) {
        switch (c) {

        case 'A': /* Hidden Autolab driver argument */
            autograder = true;
            break;
        case 'p':
            checkpoint = true;
            break;

        case 'f': /* Use one specific trace file only (relative to curr dir) */
            add_tracefile(optarg);
            strcpy(tracedir, "./");
            break;

        case 'c': /* Use one specific trace file and run only once */
            add_tracefile(optarg);
            onetime_flag = true;
            strcpy(tracedir, "./");
            break;

        case 't': /* Directory where the traces are located */
            if (num_global_tracefiles == 1) /* ignore if -f already encountered */
                break;
            strcpy(tracedir, optarg);
            if (tracedir[strlen(tracedir)-1] != '/')
                strcat(tracedir, "/"); /* path always ends with "/" */
            break;

        case 'l': /* Run libc malloc */
            run_libc = true;
            break;

        case 'V': /* Increase verbosity level */
            verbose += 1;
            break;

        case 'v': /* Set verbosity level */
            verbose = atoi(optarg);
            break;

        case 'd':
            debug_mode = atoi(optarg);
            break;

        case 'D':
            debug_mode = DBG_EXPENSIVE;
            break;

        case 's':
            set_timeout = atoi(optarg);
            break;

        case 'T':
            tab_mode = true;
            break;

        case 'h': /* Print this message */
            usage(argv[0]);
            exit(0);

        default:
            usage(argv[0]);
            exit(1);
        }
    }
#endif /* !REF_ONLY */

    if (num_global_tracefiles == 0) {
        int i;
        if (sparse_mode & !run_libc) {
            for (i = 0; default_giant_tracefiles[i]; i++)
                add_tracefile(default_giant_tracefiles[i]);
        }
        for (i = 0; default_tracefiles[i]; i++)
            add_tracefile(default_tracefiles[i]);
    }

    if (debug_mode != DBG_NONE) {
        init_random_data();
    }

    /* Initialize the timeout */
    if (set_timeout > 0) {
        signal(SIGALRM, timeout_handler);
        alarm(set_timeout);
    }

    /*
     * Optionally run and evaluate the libc malloc package
     */
    if (run_libc) {
        if (verbose > 1)
            printf("\nTesting libc malloc\n");

        /* Allocate libc stats array, with one stats_t struct per tracefile */
        libc_stats = (stats_t *)calloc(num_global_tracefiles, sizeof(stats_t));
        if (libc_stats == NULL)
            unix_error("libc_stats calloc in main failed");

        /* Evaluate the libc malloc package using the K-best scheme */
        for (i=0; i < num_global_tracefiles; i++) {
            trace_t *trace = read_trace(&libc_stats[i], tracedir, global_tracefiles[i]);

            if (verbose > 1)
                printf("Checking libc malloc for correctness, ");
            libc_stats[i].valid = eval_libc_valid(trace);
            if (libc_stats[i].valid) {
                speed_params.trace = trace;
                if (verbose > 1)
                    printf("and performance.\n");
                libc_stats[i].secs = fsec(eval_libc_speed, &speed_params);
            }
            free_trace(trace);
        }

        /* Display the libc results in a compact table and return the
           summary statistics */
        if (verbose) {
            printf("\nResults for libc malloc:\n");
            printresults(num_global_tracefiles, libc_stats, &global_libc_sum_stats);
        }
    }

#if !REF_ONLY
    /*
     * Get benchmark throughput
     */
    double ref_throughput = measure_ref_throughput(checkpoint);

    min_throughput = ref_throughput *
        (checkpoint ? MIN_SPEED_RATIO_CHECKPOINT : MIN_SPEED_RATIO);

    max_throughput = ref_throughput *
        (checkpoint ? MAX_SPEED_RATIO_CHECKPOINT : MAX_SPEED_RATIO);

    if (verbose > 0) {
        printf("Throughput targets: min=%.0f, max=%.0f, benchmark=%.0f\n",
               min_throughput, max_throughput, ref_throughput);
    }
#endif

    /*
     * Always run and evaluate the student's mm package
     */
    if (verbose > 1)
        printf("\nTesting mm malloc\n");

    /* Allocate the mm stats array, with one stats_t struct per tracefile */
    mm_stats = (stats_t *)calloc(num_global_tracefiles, sizeof(stats_t));
    if (mm_stats == NULL)
        unix_error("mm_stats calloc in main failed");

    run_tests(num_global_tracefiles, tracedir, global_tracefiles, mm_stats,
              &speed_params);


    /* Display the mm results in a compact table */
    if (verbose) {
        if (onetime_flag) {
            printf("\n\ncorrectness check finished, by running tracefile \"%s\".\n",
                   global_tracefiles[num_global_tracefiles-1]);
            if (mm_stats[num_global_tracefiles-1].valid) {
                printf(" => correct.\n\n");
            } else {
                printf(" => incorrect.\n\n");
            }
        } else {
            printf("\nResults for mm malloc:\n");
            printresults(num_global_tracefiles, mm_stats, &global_mm_sum_stats);
            printf("\n");
        }
    }

    /* Optionally compare the performance of mm and libc */
    if (run_libc) {
        printf("Comparison with libc malloc: mm/libc = %.0f Kops / %.0f Kops = %.2f\n",
               (float)global_mm_sum_stats.tput, (float)global_libc_sum_stats.tput,
               (float)(global_mm_sum_stats.tput/global_libc_sum_stats.tput));
    }

    /* temporaries used to compute the performance index */
    double avg_mm_util = 0.0;
    double avg_mm_geom_throughput = 0.0;
    double p1, p1_checkpoint;  // util index
    double p2, p2_checkpoint;  // throughput index
    double perfindex, perfindex_checkpoint;
    int util_weight = 0;
    int perf_weight = 0;

    /*
     * Accumulate the aggregate statistics for the student's mm package
     */
    double secs = 0.0;
    double ops = 0.0;
    double util = 0.0;
    double tput_geom = 1.0;
    int numcorrect = 0;

    /*
     * trace weight:
     * weight 0 => ignore
     *        1 => count both util and perf
     *        2 => count only util
     *        3 => count only perf
     */
    for (i=0; i < num_global_tracefiles; i++) {
        if (mm_stats[i].weight == WALL || mm_stats[i].weight == WPERF)
        {
            secs += mm_stats[i].secs;
            ops += mm_stats[i].ops;
            perf_weight++;
        }
        if (mm_stats[i].weight == WALL || mm_stats[i].weight == WUTIL)
        {
            util += mm_stats[i].util;
            util_weight++;
        }
        if (mm_stats[i].valid)
        {
            numcorrect++;
        }
    }

    /*
     * Calulate geometric throughput. Since we do not know the number of
     * throughput-weighted tracefiles beforehand, we need to re-loop and
     * calculate the throughputs.
     */
    for (i = 0; i < num_global_tracefiles; i++)
    {
        if (mm_stats[i].weight == WALL || mm_stats[i].weight == WPERF)
        {
            tput_geom *= pow(mm_stats[i].tput, 1./perf_weight);
        }
    }

    if (util_weight == 0) {
        avg_mm_util = 0.0;
    } else {
        avg_mm_util = util/util_weight;
    }

    /*
     * Compute and print the performance index
     */
    if (errors == 0) {
        if (sparse_mode || perf_weight == 0)
        {
            avg_mm_geom_throughput = 0.0;
        } else {
            /* Measure in KOPS */
            avg_mm_geom_throughput = tput_geom;
        }

        /* p1 - utilization */
        p1 = UTIL_WEIGHT *
            compute_scaled_score(avg_mm_util,
                MIN_SPACE, MAX_SPACE);
        p1_checkpoint = UTIL_WEIGHT_CHECKPOINT *
            compute_scaled_score(avg_mm_util,
                MIN_SPACE_CHECKPOINT, MAX_SPACE_CHECKPOINT);

        /* p2 - throughput index */
        p2 = (1.0 - UTIL_WEIGHT) *
            compute_scaled_score(avg_mm_geom_throughput,
                min_throughput, max_throughput);
        p2_checkpoint = (1.0 - UTIL_WEIGHT_CHECKPOINT) *
            compute_scaled_score(avg_mm_geom_throughput,
                min_throughput, max_throughput);

        perfindex = (p1 + p2) * 100.0;
        perfindex_checkpoint = (p1_checkpoint + p2_checkpoint) * 100.0;

#if REF_ONLY
        printf("%.0f\n", avg_mm_geom_throughput);
#else /* !REF_ONLY */
        printf("Average utilization = %.1f%%.\n", avg_mm_util * 100);

        // Don't measure throughput in sparse mode
        if (!sparse_mode) {
            printf("Average throughput (Kops/sec) = %.0f.\n",
                   avg_mm_geom_throughput);
            if (checkpoint) {
                printf("Checkpoint Perf index = %.1f (util) + %.1f (thru) = %.1f/100\n",
                       p1_checkpoint*100,
                       p2_checkpoint*100,
                       perfindex_checkpoint);
            } else {
                printf("Perf index = %.1f (util) + %.1f (thru) = %.1f/100\n",
                       p1*100,
                       p2*100,
                       perfindex);
            }
        }
#endif

    }
    else { /* There were errors */
        perfindex = 0.0;
        perfindex_checkpoint = 0.0;
        printf("Terminated with %d errors\n", errors);
    }

    /* Optionally emit autoresult string */
    double score = checkpoint ? perfindex_checkpoint : perfindex;
    /* Scoreboard shows: score, deductions, throughput, utilization */
    if (autograder) {
        sprintf(autoresult,
                "{\"scores\": {\"Autograded Score\": %.1f}, \"scoreboard\": [%.1f, %.0f, %.0f, %.1f]}",
                score, score, 0.0,
                avg_mm_geom_throughput, avg_mm_util*100);
        printf("%s\n", autoresult);
    }
    exit(0);
}


/*****************************************************************
 * Add trace to global list of tracefiles
 ****************************************************************/
static void add_tracefile(char *trace) {
    global_tracefiles = realloc(global_tracefiles,
                                (num_global_tracefiles+1) * sizeof(char *));
    global_tracefiles[num_global_tracefiles++] = strdup(trace);
}



/*****************************************************************
 * The following routines manipulate the range list, which keeps
 * track of the extent of every allocated block payload. We use the
 * range list to detect any overlapping allocated blocks.
 ****************************************************************/

/*
 * new_range_set - Create an empty range set
 */
static range_set_t *new_range_set() {
    range_set_t *ranges = (range_set_t *) malloc(sizeof(range_set_t));
    ranges->list = NULL;
    ranges->lo_tree = tree_new();
    return ranges;
}

/*
 * add_range - As directed by request opnum in trace tracenum,
 *     we've just called the student's mm_malloc to allocate a block of
 *     size bytes at addr lo. After checking the block for correctness,
 *     we create a range struct for this block and add it to the range list.
 */
static bool add_range(range_set_t *ranges, char *lo, size_t size,
                      const trace_t *trace, int opnum, int index) {
    char *hi = lo + size - 1;

    assert(size > 0);

    /* Payload addresses must be ALIGNMENT-byte aligned */
    if (!IS_ALIGNED(lo)) {
        malloc_error(trace, opnum,
                     "Payload address (%p) not aligned to %d bytes", lo, ALIGNMENT);
        return false;
    }

    /* The payload must lie within the extent of the heap */
    if ((lo < (char *)mem_heap_lo()) || (lo > (char *)mem_heap_hi()) ||
        (hi < (char *)mem_heap_lo()) || (hi > (char *)mem_heap_hi())) {
        malloc_error(trace, opnum,
                     "Payload (%p:%p) lies outside heap (%p:%p)",
                     lo, hi, mem_heap_lo(), mem_heap_hi());
        return false;
    }

    /* If we can't afford the linear-time loop, we check less thoroughly and
       just assume the overlap will be caught by writing random bits. */
    if (debug_mode == DBG_NONE) return 1;

    /* Look in the tree for the predecessor block */
    range_t *prev = tree_find_nearest(ranges->lo_tree, (long unsigned) lo);
    range_t *next = prev ? prev->next : NULL;
    /* See if it overlaps previous or next blocks */
    if (prev && lo <= prev->hi) {
        malloc_error(trace, opnum,
                     "Payload (%p:%p) overlaps another payload (%p:%p)\n",
                     lo, hi, prev->lo, prev->hi);
        return false;
    }
    if (next && hi >= next->lo) {
        malloc_error(trace, opnum,
                     "Payload (%p:%p) overlaps another payload (%p:%p)\n",
                     lo, hi, next->lo, next->hi);
        return false;
    }
    /*
     * Everything looks OK, so remember the extent of this block
     * by creating a range struct and adding it the range list.
     */
    range_t *p;
    if ((p = (range_t *)malloc(sizeof(range_t))) == NULL)
        unix_error("malloc error in add_range");
    p->prev = prev;
    if (prev)
        prev->next = p;
    else
        ranges->list = p;
    p->next = next;
    if (next)
        next->prev = p;
    p->lo = lo;
    p->hi = hi;
    p->index = index;
    tree_insert(ranges->lo_tree, (long unsigned) lo, (void *) p);
    return true;
}

/*
 * remove_range - Free the range record of block whose payload starts at lo
 */
static void remove_range(range_set_t *ranges, char *lo)
{
    range_t *p = (range_t *) tree_remove(ranges->lo_tree, (long unsigned) lo);
    if (!p)
        return;
    range_t *prev = p->prev;
    range_t *next = p->next;
    if (prev)
        prev->next = next;
    else
        ranges->list = next;
    if (next)
        next->prev = prev;
    free(p);
}

/*
 * free_range_set - free all of the range records for a trace
 */
static void free_range_set(range_set_t *ranges)
{
    tree_free(ranges->lo_tree, free);
    free(ranges);
}

/**********************************************
 * The following routines handle the random data used for
 * checking memory access.
 *********************************************/

static void init_random_data(void) {
    int len;

    if (debug_mode == DBG_NONE) return;

    for(len = 0; len < RANDOM_DATA_LEN; ++len) {
        random_data[len] = random();
    }
}

static void randomize_block(trace_t *traces, int index) {
    size_t size, fsize;
    size_t i;
    randint_t *block;
    int base;

    if (debug_mode == DBG_NONE) return;

    traces->block_rand_base[index] = random();

    block = (randint_t*)traces->blocks[index];
    size = traces->block_sizes[index] / sizeof(*block);
    if (size == 0)
        return;
    fsize = size;
    if (fsize > maxfill)
        fsize = maxfill;
    base = traces->block_rand_base[index];

    // NOTE: It's expensive to do this one byte at a time.

    // NOTE: It would be nice to also fill in at end of block, but
    // this gets messy with REALLOC

    for(i = 0; i < fsize; i++) {
        mem_write(&block[i],
                  random_data[(base + i) % RANDOM_DATA_LEN],
                  sizeof(randint_t));
    }
}

static bool check_index(const trace_t *trace, int opnum, int index) {
    size_t size, fsize;
    size_t i;
    randint_t *block;
    int base;
    int ngarbled = 0;
    int firstgarbled = -1;

    if (index < 0) return true; /* we're doing free(NULL) */
    if (debug_mode == DBG_NONE) return true;

    block = (randint_t*)trace->blocks[index];
    size = trace->block_sizes[index] / sizeof(*block);
    if (size == 0)
        return true;
    fsize = size;

    size_t thresh = maxfill;
    if (fsize > thresh)
        fsize = thresh;

    base = trace->block_rand_base[index];

    // NOTE: It's expensive to do this one byte at a time.
    for(i = 0; i < fsize; i++) {
        if (mem_read(&block[i], sizeof(randint_t)) != random_data[(base + i) % RANDOM_DATA_LEN]) {
            if (firstgarbled == -1) firstgarbled = i;
            ngarbled++;
        }
    }
    if (ngarbled != 0) {
        malloc_error(trace, opnum, "block %d (at %p) has %d garbled %s%s, "
                     "starting at byte %zu", index, &block[firstgarbled], ngarbled, randint_t_name,
                     (ngarbled > 1 ? "s" : ""), sizeof(randint_t) * firstgarbled);
        return false;
    }
    return true;
}

/**********************************************
 * The following routines manipulate tracefiles
 *********************************************/

/*
 * read_trace - read a trace file and store it in memory
 */
static trace_t *read_trace(stats_t *stats, const char *tracedir,
                           const char *filename)
{
    FILE *tracefile;
    trace_t *trace;
    char type[MAXLINE];
    int index;
    size_t size;
    int max_index = 0;
    int op_index;
    int ignore = 0;

    if (verbose > 1)
        printf("Reading tracefile: %s\n", filename);

    /* Allocate the trace record */
    if ((trace = (trace_t *) malloc(sizeof(trace_t))) == NULL)
        unix_error("malloc 1 failed in read_trace");

    /* Read the trace file header */
    strcpy(trace->filename, tracedir);
    strcat(trace->filename, filename);
    if ((tracefile = fopen(trace->filename, "r")) == NULL) {
        unix_error("Could not open %s in read_trace", trace->filename);
    }
    int iweight;
    ignore += fscanf(tracefile, "%d", &iweight);
    trace->weight = iweight;
    ignore += fscanf(tracefile, "%d", &trace->num_ids);
    ignore +=  fscanf(tracefile, "%d", &trace->num_ops);
    ignore +=  fscanf(tracefile, "%zd", &trace->data_bytes);

    if (trace->weight < 0 || trace->weight > 3) {
        app_error("%s: weight can only be in {0, 1, 2 3}", trace->filename);
    }

    /* We'll store each request line in the trace in this array */
    if ((trace->ops =
         (traceop_t *)malloc(trace->num_ops * sizeof(traceop_t))) == NULL)
        unix_error("malloc 2 failed in read_trace");

    /* We'll keep an array of pointers to the allocated blocks here... */
    if ((trace->blocks =
         (char **)calloc(trace->num_ids, sizeof(char *))) == NULL)
        unix_error("malloc 3 failed in read_trace");

    /* ... along with the corresponding byte sizes of each block */
    if ((trace->block_sizes =
         (size_t *)calloc(trace->num_ids,  sizeof(size_t))) == NULL)
        unix_error("malloc 4 failed in read_trace");

    /* and, if we're debugging, the offset into the random data */
    if ((trace->block_rand_base =
         calloc(trace->num_ids, sizeof(*trace->block_rand_base))) == NULL)
        unix_error("malloc 5 failed in read_trace");


    /* read every request line in the trace file */
    index = 0;
    op_index = 0;
    while (fscanf(tracefile, "%s", type) != EOF) {
        switch(type[0]) {
        case 'a':
            ignore += fscanf(tracefile, "%u %lu", &index, &size);
            trace->ops[op_index].type = ALLOC;
            trace->ops[op_index].index = index;
            trace->ops[op_index].size = size;
            max_index = (index > max_index) ? index : max_index;
            break;
        case 'r':
            ignore += fscanf(tracefile, "%u %lu", &index, &size);
            trace->ops[op_index].type = REALLOC;
            trace->ops[op_index].index = index;
            trace->ops[op_index].size = size;
            max_index = (index > max_index) ? index : max_index;
            break;
        case 'f':
            ignore += fscanf(tracefile, "%u", &index);
            trace->ops[op_index].type = FREE;
            trace->ops[op_index].index = index;
            break;
        default:
            app_error("Bogus type character (%c) in tracefile %s\n",
                      type[0], trace->filename);
        }
        op_index++;
        if (op_index == trace->num_ops) break;
    }
    fclose(tracefile);
    assert(max_index == trace->num_ids - 1);
    assert(trace->num_ops == op_index);

    /* fill in the stats */
    strcpy(stats->filename, trace->filename);
    stats->weight = trace->weight;
    stats->ops = trace->num_ops;

    return trace;
}

/*
 * reinit_trace - get the trace ready for another run.
 */
static void reinit_trace(trace_t *trace)
{
    memset(trace->blocks, 0, trace->num_ids * sizeof(*trace->blocks));
    memset(trace->block_sizes, 0, trace->num_ids * sizeof(*trace->block_sizes));
    /* block_rand_base is unused if size is zero */
}

/*
 * free_trace - Free the trace record and the four arrays it points
 *              to, all of which were allocated in read_trace().
 */
static void free_trace(trace_t *trace)
{
    free(trace->ops);         /* free the three arrays... */
    free(trace->blocks);
    free(trace->block_sizes);
    free(trace->block_rand_base);
    free(trace);              /* and the trace record itself... */
}

/**********************************************************************
 * The following functions evaluate the correctness, space utilization,
 * and throughput of the libc and mm malloc packages.
 **********************************************************************/

/*
 * eval_mm_valid - Check the mm malloc package for correctness
 */
static bool eval_mm_valid(trace_t *trace, range_set_t *ranges)
{
    int i;
    int index;
    size_t size;
    char *newp;
    char *oldp;
    char *p;
    bool allCheck = true;

    /* Reset the heap and free any records in the range list */
    mem_reset_brk();
    reinit_trace(trace);

    /* Call the mm package's init function */
    if (!mm_init()) {
        malloc_error(trace, 0, "mm_init failed.");
        return false;
    }

    /* Interpret each operation in the trace in order */
    for (i = 0;  i < trace->num_ops;  i++) {
        index = trace->ops[i].index;
        size = trace->ops[i].size;

        if (debug_mode == DBG_EXPENSIVE) {
            range_t *r;

            /* Let the students check their own heap */
            if (!mm_checkheap(0)) {
                malloc_error(trace, i, "mm_checkheap returned false\n");
                return false;
            };

            /* Now check that all our allocated blocks have the right data */
            r = ranges->list;
            while (r) {
                if (!check_index(trace, i, r->index))
                {
                    allCheck = false;
                }
                r = r->next;
            }
        }

        switch (trace->ops[i].type) {

        case ALLOC: /* mm_malloc */

            /* Call the student's malloc */
            if ((p = mm_malloc(size)) == NULL) {
                malloc_error(trace, i, "mm_malloc failed.");
                return false;
            }

            /*
             * Test the range of the new block for correctness and add it
             * to the range list if OK. The block must be  be aligned properly,
             * and must not overlap any currently allocated block.
             */
            if (add_range(ranges, p, size, trace, i, index) == 0)
                return false;

            /* Remember region */
            trace->blocks[index] = p;
            trace->block_sizes[index] = size;

            /* Set to random data, for debugging. */
            randomize_block(trace, index);
            break;

        case REALLOC: /* mm_realloc */
            if (!check_index(trace, i, index))
            {
                allCheck = false;
            }

            /* Call the student's realloc */
            oldp = trace->blocks[index];
            newp = mm_realloc(oldp, size);
            if ( (newp == NULL) && (size != 0) ) {
                malloc_error(trace, i, "mm_realloc failed.");
                return false;
            }
            if ( (newp != NULL) && (size == 0) ) {
                malloc_error(trace, i, "mm_realloc with size 0 returned "
                             "non-NULL.");
                return false;
            }

            /* Remove the old region from the range list */
            remove_range(ranges, oldp);

            /* Check new block for correctness and add it to range list */
            if (size > 0) {
                if (add_range(ranges, newp, size, trace, i, index) == 0)
                    return false;
            }


            /* Move the region from where it was.
             * Check up to min(size, oldsize) for correct copying. */
            trace->blocks[index] = newp;
            if (size < trace->block_sizes[index]) {
                trace->block_sizes[index] = size;
            }
            // NOTE: Might help to pass old size here to check bytes at each end of allocation

            if (!check_index(trace, i, index))
            {
                allCheck = false;
            }
            trace->block_sizes[index] = size;

            /* Set to random data, for debugging. */
            randomize_block(trace, index);
            break;

        case FREE: /* mm_free */
            if (!check_index(trace, i, index))
            {
                allCheck = false;
            }

            /* Remove region from list and call student's free function */
            if (index == -1) {
                p = 0;
            } else {
                p = trace->blocks[index];
                remove_range(ranges, p);
            }
            mm_free(p);
            break;

        default:
            app_error("Nonexistent request type in eval_mm_valid");
        }
    }
    /* As far as we know, this is a valid malloc package */
    return allCheck;
}

/*
 * eval_mm_util - Evaluate the space utilization of the student's package
 *   The idea is to remember the high water mark "hwm" of the heap for
 *   an optimal allocator, i.e., no gaps and no internal fragmentation.
 *   Utilization is the ratio hwm/heapsize, where heapsize is the
 *   size of the heap in bytes after running the student's malloc
 *   package on the trace. Note that our implementation of mem_sbrk()
 *   doesn't allow the students to decrement the brk pointer, so brk
 *   is always the high water mark of the heap.
 *
 *   A higher number is better: 1 is optimal.
 */
static double eval_mm_util(trace_t *trace, int tracenum)
{
    int i;
    int index;
    size_t size, newsize, oldsize;
    size_t max_total_size = 0;
    size_t total_size = 0;
    char *p;
    char *newp, *oldp;

    reinit_trace(trace);

    /* initialize the heap and the mm malloc package */
    mem_reset_brk();
    if (!mm_init())
        app_error("trace %d: mm_init failed in eval_mm_util", tracenum);

    for (i = 0;  i < trace->num_ops;  i++) {
        switch (trace->ops[i].type) {

        case ALLOC: /* mm_alloc */
            index = trace->ops[i].index;
            size = trace->ops[i].size;

            if ((p = mm_malloc(size)) == NULL) {
                app_error("trace %d: mm_malloc failed in eval_mm_util",
                          tracenum);
            }

            /* Remember region and size */
            trace->blocks[index] = p;
            trace->block_sizes[index] = size;

            total_size += size;
            break;

        case REALLOC: /* mm_realloc */
            index = trace->ops[i].index;
            newsize = trace->ops[i].size;
            oldsize = trace->block_sizes[index];

            oldp = trace->blocks[index];
            if ((newp = mm_realloc(oldp,newsize)) == NULL && newsize != 0) {
                app_error("trace %d: mm_realloc failed in eval_mm_util",
                          tracenum);
            }

            /* Remember region and size */
            trace->blocks[index] = newp;
            trace->block_sizes[index] = newsize;

            total_size += (newsize - oldsize);
            break;

        case FREE: /* mm_free */
            index = trace->ops[i].index;
            if (index < 0) {
                size = 0;
                p = 0;
            } else {
                size = trace->block_sizes[index];
                p = trace->blocks[index];
            }

            mm_free(p);

            total_size -= size;
            break;

        default:
            app_error("trace %d: Nonexistent request type in eval_mm_util",
                      tracenum);
        }

        /* update the high-water mark */
        max_total_size = (total_size > max_total_size) ?
            total_size : max_total_size;
    }

#if !REF_ONLY
    printf(".");
#endif

    return ((double)max_total_size / (double)mem_heapsize());
}


/*
 * eval_mm_speed - This is the function that is used by fcyc()
 *    to measure the running time of the mm malloc package.
 */
static void eval_mm_speed(void *ptr)
{
    int i, index;
    size_t size, newsize;
    char *p, *newp, *oldp, *block;
    trace_t *trace = ((speed_t *)ptr)->trace;
    reinit_trace(trace);

    /* Reset the heap and initialize the mm package */
    mem_reset_brk();
    if (!mm_init())
        app_error("mm_init failed in eval_mm_speed");

    /* Interpret each trace request */
    for (i = 0;  i < trace->num_ops;  i++)
        switch (trace->ops[i].type) {

        case ALLOC: /* mm_malloc */
            index = trace->ops[i].index;
            size = trace->ops[i].size;
            if ((p = mm_malloc(size)) == NULL)
                app_error("mm_malloc error in eval_mm_speed");
            trace->blocks[index] = p;
            break;

        case REALLOC: /* mm_realloc */
            index = trace->ops[i].index;
            newsize = trace->ops[i].size;
            oldp = trace->blocks[index];
            if ((newp = mm_realloc(oldp,newsize)) == NULL && newsize != 0)
                app_error("mm_realloc error in eval_mm_speed");
            trace->blocks[index] = newp;
            break;

        case FREE: /* mm_free */
            index = trace->ops[i].index;
            if (index < 0) {
                block = 0;
            } else {
                block = trace->blocks[index];
            }
            mm_free(block);
            break;

        default:
            app_error("Nonexistent request type in eval_mm_speed");
        }
}

/*
 * eval_libc_valid - We run this function to make sure that the
 *    libc malloc can run to completion on the set of traces.
 *    We'll be conservative and terminate if any libc malloc call fails.
 *
 */
static bool eval_libc_valid(trace_t *trace)
{
    int i;
    size_t newsize;
    char *p, *newp, *oldp;

    reinit_trace(trace);

    for (i = 0;  i < trace->num_ops;  i++) {
        switch (trace->ops[i].type) {

        case ALLOC: /* malloc */
            if ((p = malloc(trace->ops[i].size)) == NULL) {
                malloc_error(trace, i, "libc malloc failed");
                unix_error("System message");
            }
            trace->blocks[trace->ops[i].index] = p;
            break;

        case REALLOC: /* realloc */
            newsize = trace->ops[i].size;
            oldp = trace->blocks[trace->ops[i].index];
            if ((newp = realloc(oldp, newsize)) == NULL && newsize != 0) {
                malloc_error(trace, i, "libc realloc failed");
                unix_error("System message");
            }
            trace->blocks[trace->ops[i].index] = newp;
            break;

        case FREE: /* free */
            if (trace->ops[i].index >= 0) {
                free(trace->blocks[trace->ops[i].index]);
            } else {
                free(0);
            }
            break;

        default:
            app_error("invalid operation type  in eval_libc_valid");
        }
    }

    return true;
}

/*
 * eval_libc_speed - This is the function that is used by fcyc() to
 *    measure the running time of the libc malloc package on the set
 *    of traces.
 */
static void eval_libc_speed(void *ptr)
{
    int i;
    int index;
    size_t size, newsize;
    char *p, *newp, *oldp, *block;
    trace_t *trace = ((speed_t *)ptr)->trace;

    reinit_trace(trace);

    for (i = 0;  i < trace->num_ops;  i++) {
        switch (trace->ops[i].type) {
        case ALLOC: /* malloc */
            index = trace->ops[i].index;
            size = trace->ops[i].size;
            if ((p = malloc(size)) == NULL)
                unix_error("malloc failed in eval_libc_speed");
            trace->blocks[index] = p;
            break;

        case REALLOC: /* realloc */
            index = trace->ops[i].index;
            newsize = trace->ops[i].size;
            oldp = trace->blocks[index];
            if ((newp = realloc(oldp, newsize)) == NULL && newsize != 0)
                unix_error("realloc failed in eval_libc_speed\n");

            trace->blocks[index] = newp;
            break;

        case FREE: /* free */
            index = trace->ops[i].index;
            if (index >= 0) {
                block = trace->blocks[index];
                free(block);
            } else {
                free(0);
            }
            break;
        }
    }
}

/*************************************
 * Some miscellaneous helper routines
 ************************************/


/*
 * printresults - prints a performance summary for some malloc package and returns
 *                a summary of the stats to the caller.
 */
static void printresults(int n, stats_t *stats, sum_stats_t *sumstats)
{
    int i;

    /* weighted sums all */
    double sumsecs = 0;
    double sumops  = 0;
    double sumtput = 0;
    double sumutil = 0;
    int sum_perf_weight = 0;
    int sum_util_weight = 0;

    char wstr;
    char *tabstr;

    /* Print the individual results for each trace */
    if (tab_mode) {
        printf("valid\tthru?\tutil?\tutil\tops\tmsecs\tKops/s\ttrace\n");
    } else {
        printf("  %5s  %6s %7s%8s%8s  %s\n",
               "valid", "util", "ops", "msecs", "Kops/s", "trace");
    }
    for (i=0; i < n; i++) {
        if (stats[i].valid) {
            switch(stats[i].weight)
                {
                case WNONE:
                    wstr = ' ';
                    tabstr = "0\t0\t";
                    break;
                case WALL:
                    wstr = '*';
                    tabstr = "1\t1\t";
                    break;
                case WUTIL:
                    wstr = 'u';
                    tabstr = "0\t1\t";
                    break;
                case WPERF:
                    wstr = 'p';
                    tabstr = "1\t0\t";
                    break;
                default:
                    app_error("wrong value for weight found!");
                }

            /* Valid = whether performance and/or throughput counted */
            if (tab_mode) {
                printf("1\t%s", tabstr);
            } else {
                /* prints done in a somewhat silly way to avoid hassle
                 * if future columns need to be added/modified like this time */
                printf("%2c", wstr);
                printf("%4s", "yes");
            }

            /* Utilization */
            if (tab_mode) {
                printf("%.1f\t", stats[i].util * 100.0);
            } else {
                /* print '--' if util isn't weighted */
                if (stats[i].weight == WNONE || stats[i].weight == WALL
                    || stats[i].weight == WUTIL)
                    printf(" %7.1f%%", stats[i].util * 100.0);
                else
                    printf(" %8s", "--");
            }

            /* Ops + Time */
            double msecs = sparse_mode ? 0.0 : stats[i].secs * 1000.0;
            double kops = sparse_mode ? 0.0 : stats[i].tput;
            if (tab_mode) {
                printf("%.0f\t%.3f\t%.0f\t",
                       stats[i].ops, msecs, kops);
            } else {
                /* print '--' if perf isn't weighted */
                if (stats[i].weight == WNONE || stats[i].weight == WALL
                    || stats[i].weight == WPERF)
                    printf("%8.0f%10.3f%7.0f ", stats[i].ops, msecs, kops);
                else
                    printf("%8s%10s%7s ", "--", "--", "--");
            }

            printf("%s\n", stats[i].filename);

            if (stats[i].weight == WALL || stats[i].weight == WPERF)
                {
                    sum_perf_weight += 1;
                    sumsecs += stats[i].secs;
                    sumops += stats[i].ops;
                    sumtput += stats[i].tput;
                }
            if (stats[i].weight == WALL || stats[i].weight == WUTIL)
                {
                    sum_util_weight += 1;
                    sumutil += stats[i].util;
                }
        }
        else {
            if (tab_mode) {
                printf("no\t\t\t\t\t\t\t%s\n", stats[i].filename);
            } else {
                printf("%2s%4s%7s%10s%7s%10s %s\n",
                       stats[i].weight != 0 ? "*" : "",
                       "no",
                       "-",
                       "-",
                       "-",
                       "-",
                       stats[i].filename);
            }
        }
    }

    /* Print the aggregate results for the set of traces */
    if (errors == 0) {
        if (sum_perf_weight == 0)
            sum_perf_weight = 1;
        if (sum_util_weight == 0)
            sum_util_weight = 1;

        double util = sumutil / (double)sum_util_weight;
        double tput = sparse_mode ? 0.0 : sumtput / (double)sum_perf_weight;
        if (sparse_mode)
            sumsecs = 0;
        if (tab_mode) {
            // "valid\tthru?\tutil?\tutil\tops\tmsecs\tKops\ttrace"
            printf("Sum\t%d\t%d\t%.1f\t%.0f\t\%.2f\n",
                   sum_perf_weight,
                   sum_util_weight,
                   sumutil * 100.0,
                   sumops,
                   sumsecs * 1000.0);
            printf("Avg\t\t\t%.1f\t\t\t\n",
                   util * 100.0);
        } else {
            printf("%2d %2d  %7.1f%%%8.0f%10.3f\n",
                   sum_util_weight,
                   sum_perf_weight,
                   util * 100.0,
                   sumops,
                   sumsecs * 1000.0);
        }

        /* Record the summary statistics so we can compare libc and
           mm.cc */
        sumstats->util = util;
        sumstats->ops = sumops;
        sumstats->secs = sumsecs;
        sumstats->tput = tput;
    }
    else {
        if (!tab_mode) {
            printf("     %8s%10s%7s\n",
                   "-",
                   "-",
                   "-");
        }

        /* Record the summary statistics so we can compare libc and
           mm.c */
        sumstats->util = 0;
        sumstats->ops = 0;
        sumstats->secs = 0;
        sumstats->tput = 0;
    }
}

/*
 * app_error - Report an arbitrary application error
 */
void app_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    fflush(NULL);
    exit(1);
}

/*
 * unix_error - Report the error and its errno.
 */
void unix_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    printf(": %s\n", strerror(errno));
    va_end(ap);
    fflush(NULL);
    exit(1);
}

/*
 * malloc_error - Report an error returned by the mm_malloc package
 */
void malloc_error(const trace_t *trace, int opnum, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    errors++;

    printf("ERROR [trace %s, line %d]: ", trace->filename, LINENUM(opnum));
    vprintf(fmt, ap);
    putchar('\n');

    va_end(ap);
    fflush(NULL);
}

/*
 * compute_scaled_score: Scales a raw score in the range from lo to hi to the
 * range from 0.0 to 1.0. In other words, a raw score of lo returns 0.0, a raw
 * score of hi returns 1.0, and anything in between is interpolated.
 */
static double compute_scaled_score(double value, double lo, double hi) {
    if (value < lo) return 0.0;
    else if (value > hi) return 1.0;
    else return (value - lo) / (hi - lo);
}

/*****
 * Routines for reference throughput lookup
 *****/

/*
 * cparse.  Rewrite string by removing whitespace and replacing ':' with NULL.
 * Second argument filled with starting addresses of the resulting substrings
 * Return number of tokens generated
 */

#define PLIMIT 10
static int cparse(char *s, char **index) {
    int found = 0;
    bool done = false;
    char *last_start = s;
    char *rpos = s;
    char *wpos = s;
    while (found < PLIMIT && !done) {
        int c = *rpos++;
        switch (c) {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            /* Skip */
            break;
        case ':':
            /* Finish off current token */
            *wpos++ = 0;
            index[found++] = last_start;
            last_start = wpos;
            break;
        case 0:
            /* End of string.  Finish off token */
            *wpos = 0;
            index[found++] = last_start;
            done = true;
            break;
        default:
            *wpos++ = c;
        }
    }
    return found;
}

/* Read throughput from file */
static double lookup_ref_throughput(bool checkpoint) {
    char buf[MAXLINE];
    char *tokens[PLIMIT];
    char cpu_type[MAXLINE] = "";
    double tput = 0.0;
    char *bench_type = checkpoint ? BENCH_KEY_CHECKPOINT : BENCH_KEY;

    /* Scan file to find CPU type */
    FILE *ifile = fopen(CPU_FILE, "r");
    if (!ifile) {
        fprintf(stderr, "Warning: Could not find file '%s'\n", CPU_FILE);
        return tput;
    }
    /* Read lines in file.  Parse each one to look for key */
    bool found = false;
    while (fgets(buf, MAXLINE, ifile) != NULL) {
        int t = cparse(buf, tokens);
        if (t < 2)
            continue;
        if (strcmp(CPU_KEY, tokens[0]) == 0) {
            strcpy(cpu_type, tokens[1]);
            found = true;
            break;
        }
    }
    fclose(ifile);
    if (!found) {
        fprintf(stderr, "Warning: Could not find CPU type in file '%s'\n", CPU_FILE);
        return tput;
    }
    /* Now try to find matching entry in throughput file */
    FILE *tfile = fopen(THROUGHPUT_FILE, "r");
    if (tfile == NULL) {
        fprintf(stderr, "Warning: Could not open throughput file '%s'\n", THROUGHPUT_FILE);
        return tput;
    }
    found = false;
    while (fgets(buf, MAXLINE, tfile) != NULL) {
        int t = cparse(buf, tokens);
        if (t < 3)
            continue;
        if (strcmp(tokens[0], cpu_type) == 0 &&
            strcmp(tokens[1], bench_type) == 0) {
            tput = atof(tokens[2]);
            found = true;
            break;
        }
    }
    fclose(tfile);
    if (tput == 0.0) {
        fprintf(stderr, "Warning: Could not find CPU '%s' benchmark '%s' in throughput file '%s'\n",
                cpu_type, bench_type, THROUGHPUT_FILE);
    }
    if (tput > 0.0 && verbose > 0) {
        printf("Found benchmark throughput %.0f for cpu type %s, benchmark %s\n",
               tput, cpu_type, bench_type);
    }
    return tput;
}

/*
 * gen_file_name: Generate a file name that does not currently exist
 * Give template suitable for use with sprintf, with one entry suitable for an integer value.
 * E.g., "./file_%.8x.txt".
 */
static char *gen_file_name(char *template, char *buf, int maxlen) {
    int r;
    int lim = 100;
    do {
        r = random();
        snprintf(buf, maxlen, template, r);
        lim--;
    } while (lim > 0 && access(buf, F_OK) == 0);
    return lim > 0 ? buf : NULL;
}

/*
 * measure_ref_throughput: Measure throughput achieved by reference implementation
 */
static double measure_ref_throughput(bool checkpoint) {
    double ltput = lookup_ref_throughput(checkpoint);
    if (ltput > 0)
        return ltput;
    char buf[MAXLINE];
    char cmd[MAXLINE];
    char *fname = gen_file_name("./tput_%.8x.txt", buf, MAXLINE);
    float t;
    sprintf(cmd, "%s > %s", checkpoint ? REF_DRIVER_CHECKPOINT : REF_DRIVER,
            fname);
    if (verbose > 1) {
        printf("Executing '%s'\n", cmd);
    }
    if (system(cmd) != 0) {
        fprintf(stderr, "Couldn't execute '%s'\n", cmd);
        exit(1);
    }
    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        fprintf(stderr, "Couldn't open '%s'\n", fname);
        exit(1);
    }
    if (fscanf(f, "%f", &t) != 1) {
        fprintf(stderr, "Couldn't read result from '%s'\n", fname);
        exit(1);
    }
    if (fclose(f) != 0) {
        fprintf(stderr, "Couldn't close '%s'\n", fname);
    }
    sprintf(cmd, "rm -f %s", fname);
    if (system(cmd) != 0) {
        fprintf(stderr, "Couldn't delete '%s'\n", fname);
    }
    return (double) t;
}


/*
 * usage - Explain the command line arguments
 */
static void usage(char *prog)
{
    fprintf(stderr, "Usage: %s [-hlVdD] [-f <file>]\n", prog);
    fprintf(stderr, "Options\n");
    fprintf(stderr, "\t-p         Calculate Checkpoint Score.\n");
    fprintf(stderr, "\t-d <i>     Debug: 0 off; 1 default; 2 lots.\n");
    fprintf(stderr, "\t-D         Equivalent to -d2.\n");
    fprintf(stderr, "\t-c <file>  Run trace file <file> twice, check for correctness only.\n");
    fprintf(stderr, "\t-t <dir>   Directory to find default traces.\n");
    fprintf(stderr, "\t-h         Print this message.\n");
    fprintf(stderr, "\t-l         Run libc malloc as well.\n");
    fprintf(stderr, "\t-V         Print diagnostics as each trace is run.\n");
    fprintf(stderr, "\t-v <i>     Set Verbosity Level to <i>\n");
    fprintf(stderr, "\t-s <s>     Timeout after s secs (default no timeout)\n");
    fprintf(stderr, "\t-T         Print diagnostics in tab mode\n");
    fprintf(stderr, "\t-f <file>  Use <file> as the trace file\n");
}
