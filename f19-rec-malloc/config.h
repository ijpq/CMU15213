#ifndef __CONFIG_H_
#define __CONFIG_H_

/*
 * config.h - malloc lab configuration file
 *
 * Copyright (c) 2002, R. Bryant and D. O'Hallaron, All rights reserved.
 * May not be used, modified, or copied without permission.
 */

/*
 * Sparse mode emulates a very large address space using a very sparse
 * table of "pages".  This enables testing of memory allocations that
 * would otherwise not be feasible.  Only used as a correctness test,
 * not for throughput or utilization
 */

#ifndef SPARSE_MODE
#define SPARSE_MODE 0
#endif

/*
 * This is the default path where the driver will look for the
 * default tracefiles. You can override it at runtime with the -t flag.
 */
#define TRACEDIR "./traces/"
#define OLD_TRACEDIR "./traces-old/"

/*
 * This is the list of default tracefiles in TRACEDIR that the driver
 * will use for testing. Modify this if you want to add or delete
 * traces from the driver's test suite.
 */

#define DEFAULT_TRACEFILES  \
  "syn-array-short.rep",	\
  "syn-struct-short.rep",	\
  "syn-string-short.rep",	\
  "syn-mix-short.rep",	\
  "ngram-fox1.rep", \
  "syn-mix-realloc.rep",	\
  "bdd-aa4.rep", \
  "bdd-aa32.rep", \
  "bdd-ma4.rep", \
  "bdd-nq7.rep", \
  "cbit-abs.rep", \
  "cbit-parity.rep", \
  "cbit-satadd.rep", \
  "cbit-xyz.rep", \
  "ngram-gulliver1.rep", \
  "ngram-gulliver2.rep", \
  "ngram-moby1.rep", \
  "ngram-shake1.rep", \
  "syn-array.rep", \
  "syn-mix.rep", \
  "syn-string.rep", \
  "syn-struct.rep"

#define DEFAULT_GIANT_TRACEFILES \
  "syn-giantarray-short.rep", \
  "syn-giantmix-short.rep", \
  "syn-giantarray-med.rep", \
  "syn-giantarray.rep", \
  "syn-giantmix.rep"

/*
 * Programs for measuring reference throughputs
 */
#define REF_DRIVER "./mdriver-ref"
#define REF_DRIVER_CHECKPOINT "./mdriver-cp-ref"


/*
 * Speeds measured relative to a benchmark.  Express thresholds
 * relative to benchmark throughput
 * Students get 0 points for this point or below (ops / sec)
 */
#define MIN_SPEED_RATIO       0.50
#define MIN_SPEED_RATIO_CHECKPOINT 0.20
/*
 * Students get 0 points for this allocation fraction or below
 */
#define MIN_SPACE_CHECKPOINT 0.55
#define MIN_SPACE       0.55


/* 
 * Students can get more points for building faster allocators, up to
 * this point (in ops / sec)
 */
#define MAX_SPEED_RATIO_CHECKPOINT 0.90
#define MAX_SPEED_RATIO       0.90

/* 
 * Students can get more points for building more efficient allocators,
 * up to this point (1 is perfect).
 */
#define MAX_SPACE       0.74
#define MAX_SPACE_CHECKPOINT 0.58

 /*
  * This constant determines the contributions of space utilization
  * (UTIL_WEIGHT) and throughput (1 - UTIL_WEIGHT) to the performance
  * index.
  */

#define UTIL_WEIGHT .60
#define UTIL_WEIGHT_CHECKPOINT .20

/*
 * Max number of random values written to each allocation 
*/
#define MAXFILL        2048
#define MAXFILL_SPARSE 1024

/*
 * Alignment requirement in bytes (either 4, 8, or 16)
 */
#define ALIGNMENT 16

/*********** Parameters controlling dense memory version of heap ***********/
/*
 * Maximum heap size in bytes
 */
#define MAX_DENSE_HEAP (100*(1<<20))  /* 100 MB */

/*
 * Starting address of the memory allocated for the heap by mmap
 */
#define TRY_DENSE_HEAP_START (void *) 0x800000000


/*********** Parameters controlling sparse memory version of heap ***********/

/*
 * Maximum heap size in bytes
 */
#define MAX_SPARSE_HEAP (1UL<<62)  /* 1 EB */

/*
 * Initial address of emulated heap
 */
#define SPARSE_HEAP_START (void *) 0x2130051300000000UL

/*
 * Number of bytes in each page
 */
#define SPARSE_PAGE_SIZE (1<<10)

/*
 * Maximum target load for hash table
 */
#define HASH_LOAD 10.0

/***************** Parameters for looking up reference throughput *********/
/*
 * Location of information on CPU type
 */
#define CPU_FILE "/proc/cpuinfo"

/*
 * Key in file (spaces removed)
 */
#define CPU_KEY "modelname"

/*
 * File containing throughputs
 */
#define THROUGHPUT_FILE "./throughputs.txt"

/*
 * Keys for checkpoint vs. regular
 */
#define BENCH_KEY  "regular"
#define BENCH_KEY_CHECKPOINT "checkpoint"

#endif /* __CONFIG_H */
