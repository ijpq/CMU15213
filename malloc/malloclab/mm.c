/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"
#include "macro.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "TeanOfIjpq",
    /* First member's full name */
    "ketang",
    /* First member's email address */
    "509634578tk@gmail.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/*
 * define static(private) global variables here.
 */
static char *heap_listp = NULL;
static char *implicit_free_listPtr = NULL;

/*
 * there are x helper function to help mm_checker checking 
 * whether heap is valid. both of them returning 0 means correct 
 * and -1 otherwise.
 */
static int IsFreeMarked() {
    implicit_free_listPtr = heap_listp;
    return 0;
}

static int mm_checker(void) {
    return 0;
}

/*
 * coalesce : define how we merge free blocks.
 */
static void *coalesce(void *bp) {
    size_t prev_alloc = IS_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = IS_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    
    if (prev_alloc && next_alloc) {
        return bp;
    } else if (prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size,0));
    } else if (!prev_alloc && next_alloc) {
        size += GET_SIZE(FTRP(PREV_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        bp = PREV_BLKP(bp);
    } else if (!prev_alloc && !next_alloc) {
        size_t prev_size = GET_SIZE(HDRP(PREV_BLKP(bp)));
        size_t next_size = GET_SIZE(HDRP(NEXT_BLKP(bp)));
        size += (prev_size + next_size);
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
        
    }
    return bp;
}

/*
 * invoked when no more space to place or mm_init.
 */
static void *extend_heap(size_t words) {
    //
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE; //line:vm:mm:beginextend
    if ((long)(bp = mem_sbrk(size)) == -1)  
        return NULL;                                        //line:vm:mm:endextend

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));         /* Free block header */   //line:vm:mm:freeblockhdr
    PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */   //line:vm:mm:freeblockftr
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */ //line:vm:mm:newepihdr

    /* Coalesce if the previous block was free */
    return coalesce(bp);                                          //line:vm:mm:returnblock
}

/* 
 * find_fit
 */
static void *find_fit(size_t asize) {
    
    char *walkHeapPtr = NEXT_BLKP(heap_listp);
    if (FIT_STRATEGY == 0) {
        while (GET(HDRP(walkHeapPtr)) != PACK(0, 1)) {
            if (!IS_ALLOC(HDRP(walkHeapPtr)) && \
                GET_SIZE(HDRP(walkHeapPtr))>= asize) {
                return walkHeapPtr;
            }
            walkHeapPtr = NEXT_BLKP(walkHeapPtr);
        }
        
    } else if (FIT_STRATEGY == 1) {
        //TODO impl best fit

    } else if (FIT_STRATEGY == 2) {
        //TODO impl next fit
        
    }
    return NULL;
}

/*
 * place : how we allocate a block and how we deal with the remain free space
 * within block.
 */
static void place(void *bp, size_t asize) {
    size_t allocableSize = GET_SIZE(HDRP(bp));
    size_t kMinimumSize = 16; // hard coding
    
    if (allocableSize - asize >= kMinimumSize) {
        // split block.
        size_t remainSize = allocableSize - asize;
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        PUT(HDRP(NEXT_BLKP(bp)), PACK(remainSize, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(remainSize, 0));
    } else {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        // didn't split.
    }
    return ;
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void*)-1) {
        return -1;
    }
    PUT(heap_listp, 0);
    PUT(heap_listp + 1*(WSIZE), PACK(DSIZE,1));
    PUT(heap_listp + 2*(WSIZE), PACK(DSIZE,1));
    PUT(heap_listp + 3*(WSIZE), PACK(0,1));
    heap_listp += (2*WSIZE);

    if (extend_heap(CHUNKSIZE/WSIZE) == NULL) {
        return -1;
    }
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize; // in byte
    size_t extendsize; // in byte
    char *bp;
    if (size==0) {
        return NULL;
    }
    
    if (size < DSIZE) {
        asize = 2*DSIZE;
    } else {
        asize = DSIZE * (((size+DSIZE) + DSIZE-1) / DSIZE);
    }
    
    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    extendsize = asize > CHUNKSIZE ? asize : CHUNKSIZE;
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL) {
        return NULL;
    }
    place(bp, asize);
    return bp;

}

/* 
 * mm_free - Free a block 
 */
/* $begin mmfree */
void mm_free(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp));
    if (heap_listp == 0){
        mm_init();
    }
    /* $begin mmfree */

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

/* $end mmfree */

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}
