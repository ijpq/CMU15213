/*
 * mm-naive.c - A very fast, but memory-inefficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  Blocks are never coalesced or reused.  The size of
 * a block is stored as part of the block header, since it is needed
 * for realloc.
 *
 * This code is correct and fast, but it fails on some traces, and it
 * has poor utilization
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <inttypes.h>

#include "mm.h"
#include "memlib.h"

/* Do not change the following! */

#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#define memset mem_memset
#define memcpy mem_memcpy
#endif /* def DRIVER */

/* You can change anything from here onward */

/*
 * If DEBUG is defined (such as when running mdriver-dbg), these macros
 * are enabled. You can use them to print debugging output and to check
 * contracts only in debug mode.
 *
 * Only debugging macros with names beginning "dbg_" are allowed.
 * You may not define any other macros having arguments.
 */
#ifdef DEBUG
/* When DEBUG is defined, these form aliases to useful functions */
#define dbg_printf(...)     printf(__VA_ARGS__)
#define dbg_requires(expr)  assert(expr)
#define dbg_assert(expr)    assert(expr)
#define dbg_ensures(expr)   assert(expr)
#define dbg_printheap(...)  print_heap(__VA_ARGS__)
#else
/* When DEBUG is not defined, no code gets generated for these */
/* The sizeof() hack is used to avoid "unused variable" warnings */
#define dbg_printf(...)     (sizeof(__VA_ARGS__), -1)
#define dbg_requires(expr)  (sizeof(expr), 1)
#define dbg_assert(expr)    (sizeof(expr), 1)
#define dbg_ensures(expr)   (sizeof(expr), 1)
#define dbg_printheap(...)  ((void) sizeof(__VA_ARGS__))
#endif

typedef uint64_t word_t;
#define WSIZE sizeof(word_t)

/* What is the correct alignment? */
#define ALIGNMENT (2*WSIZE)

/*
 * View memory as organized as blocks with header indicating block
 * size, and payload containing actual data
 */
typedef struct {
    word_t size;     // Total size of allocation
    word_t padding;  // To ensure alignment of payload
    char payload[0]; // Payload size is variable
} block_t;

/*
 * What is the offset of the header from the payload?
 */
#define HEADER_SIZE offsetof(block_t, payload)

/*
 * Forward declarations of support functions
 */
static block_t *payload_to_header(void *bp);
static void *header_to_payload(block_t *block);
static size_t roundup(size_t size, size_t multiple);

/*
 * mm_init - Called when a new trace starts. 
 * CAUTION: You must reset all of your global pointers here. 
 */
bool mm_init(void)
{
    return true;
}

/*
 * malloc - Allocate a block by expanding the heap
 */
void *malloc(size_t size)
{
    size_t newsize = roundup(size + HEADER_SIZE, ALIGNMENT);
    block_t *block = (block_t *) mem_sbrk(newsize);

    if (block == (void *) -1)
        return NULL;
    else {
	block->size = newsize;
	void *p = header_to_payload(block);
	dbg_printf("malloc %zu => %p\n", size, p);
        return p;
    }
}

/*
 * free - We don't know how to free a block.  So we ignore this call.
 *      Computers have big memories; surely it won't be a problem.
 */
void free(void *ptr){
    dbg_printf("free(%p)\n", ptr);
}

/*
 * realloc - Change the size of the block by mallocing a new block,
 *      copying its data, and freeing the old block.  I'm too lazy
 *      to do better.
 */
void *realloc(void *oldptr, size_t size)
{
    /* If size == 0 then this is just free, and we return NULL. */
    if (size == 0) {
        free(oldptr);
        return NULL;
    }

    /* If oldptr is NULL, then this is just malloc. */
    if (oldptr == NULL) {
        return malloc(size);
    }

    void *newptr = malloc(size);

    /* If realloc() fails the original block is left untouched  */
    if (!newptr) {
        return NULL;
    }

    /* Copy the old data. */
    block_t *block = payload_to_header(oldptr);
    size_t copysize = block->size;
    if (size < copysize)
	copysize = size;
    memcpy(newptr, oldptr, copysize);

    /* Free the old block. */
    free(oldptr);

    return newptr;
}

/*
 * calloc - Allocate the block and set it to zero.
 */
void *calloc (size_t nmemb, size_t size)
{
    size_t bytes = nmemb * size;
    void *newptr;

    newptr = malloc(bytes);
    memset(newptr, 0, bytes);

    return newptr;
}

/*
 * mm_checkheap - There are no bugs in my code, so I don't need to
 *      check, so nah! (But if I did, I could call this function using
 *      mm_checkheap(__LINE__) to identify the call site.)
 */
bool mm_checkheap(int lineno){
    dbg_printf("Checkheap called at line %d\n", lineno);
    return true;
}

/***********************************************************************
 * Support functions
 ***********************************************************************/

/* payload -> header, header -> payload functions:
 * since each block points to the header and not the payload,
 * adjustments are necessary when returning from malloc as well as
 * prior to freeing a block.
 */
static block_t *payload_to_header(void *bp)
{
    /* Can't avoid pointer arithmetic here */
    return (block_t *)(((char *)bp) - offsetof(block_t, payload));
}

static void *header_to_payload(block_t *block)
{
    return (void *)(block->payload);
}

/* Round up size to multiple of multiple */
static size_t roundup(size_t size, size_t multiple) {
    return multiple * ((size + multiple - 1)/multiple);
}
