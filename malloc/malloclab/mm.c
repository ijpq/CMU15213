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
#include "stdbool.h"
//#include "checkhelper.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "TeamOfIjpq",
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
#define AGNSIZE(size) DSIZE*(((size+DSIZE)+DSIZE-1)/DSIZE)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/*
 * define static(private) global variables here.
 */
static char *heap_listp = NULL;
static char *start_brk = NULL;
static char *final_brk = NULL;
static char *starterbp = NULL; // staring pointer at this block
static char *nxt_starterbp = NULL; // ending pointer at this block,
                                   //next starterbp as well.
/*
 * there are x helper function to help mm_checker checking 
 * whether heap is valid. both of them returning 0 means correct 
 * and -1 otherwise.
 */

/*
 * coalesce : define how we merge free blocks.
 */
static void *coalesce(void *bp) {
    if (bp == NULL) return bp;
    int prev_alloc, next_alloc;
    size_t size = GET_SIZE(HDRP(bp));
    next_alloc = IS_ALLOC(HDRP(NEXT_BLKP(bp)));
    prev_alloc = IS_ALLOC(HDRP(PREV_BLKP(bp)));

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
    
    final_brk = bp + size;

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
    
    size_t size;
    char *walkHeapPtr = NEXT_BLKP(heap_listp);
    if (FIT_STRATEGY == 0) {
        while ((size=GET_SIZE(HDRP(walkHeapPtr))) != 0) {
            if (!IS_ALLOC(HDRP(walkHeapPtr)) && \
                size>= asize) {
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
    size_t allocable_size = GET_SIZE(HDRP(bp));
    const size_t kMinSize = 2*DSIZE; // hard coding
    size_t remain_size = allocable_size - asize;
    
    if (remain_size >= kMinSize) {
        // split block.
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        PUT(HDRP(NEXT_BLKP(bp)), PACK(remain_size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(remain_size, 0));
    } else if (remain_size >= 0) {
        // remain a fragment.
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        if (((remain_size/WSIZE) & 1) == 1) {
            // because of DSIZE aligment, there should not exist 1 WSIZE or
            // 3 WSIZE.
            printf("found 1 WSIZE or 3 WSIZE fragment.\n");
            exit(-1);
        } else if (remain_size == DSIZE) {
            PUT(HDRP(NEXT_BLKP(bp)), PACK(DSIZE, 0));
            PUT(FTRP(NEXT_BLKP(bp)), PACK(DSIZE, 0));
        }
        // didn't split.
    } else {
        // allocable_size should be always larger or equals than asize
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
    start_brk = heap_listp;
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
    // parameter passed to mm_malloc must before alignment.
    size_t asize;
    size_t extendsize; // in byte
    char *bp;
    if (size==0) {
        return NULL;
    }
        
    // both HDR and FTR occupies WSIZE that's why (size+DSIZE) in alignment.
    if (size <= DSIZE) {
        asize = 2*DSIZE;
    } else {
        asize = AGNSIZE(size);
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
    if (bp == NULL) return ;
    size_t size = GET_SIZE(HDRP(bp));
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
    if (ptr == NULL) {
        return mm_malloc(size);
    }
    if (!size) {
        mm_free(ptr);
        return NULL;
    }

    void *oldptr = ptr;
    void *newptr;
    
    //if (GET_SIZE(HDRP(ptr)) != GET_SIZE(FTRP(ptr)) || IS_ALLOC(HDRP(ptr))
    //        != IS_ALLOC(FTRP(ptr))) {
    //    // ptr didn't returned by mm_realloc or mm_malloc.
    //    return NULL;
    //}

    size_t asize;
    if (size <= DSIZE) {
        asize = 2*DSIZE;
    } else {
        asize =  AGNSIZE(size);
    }

    size_t old_size = GET_SIZE(HDRP(oldptr));
    if (asize == old_size) return oldptr;
    
    size_t copy_size = GET_SIZE(HDRP(oldptr)) > asize? asize: GET_SIZE(HDRP(oldptr));

    if ((newptr = mm_malloc(size)) == NULL) {
        // mm_malloc returning NULL only if run out of memory.
        // that means the realloc is constricted impl in adjcent block.
        if (asize > old_size) {
            int next_free = !IS_ALLOC(NEXT_BLKP(oldptr));
            int prev_free = !IS_ALLOC(PREV_BLKP(oldptr));
            if (!next_free && !prev_free) {
                return NULL;
            } else if (next_free && GET_SIZE(HDRP(NEXT_BLKP(oldptr)))+old_size>=asize) {
                size_t remain_size = GET_SIZE(HDRP(NEXT_BLKP(oldptr)))+old_size-asize;
                PUT(HDRP(oldptr), PACK(asize, 1));
                PUT(FTRP(oldptr), PACK(asize, 1));
                if (remain_size >= DSIZE) { // possible 2WSIZE, 4WSIZE and larger.
                    PUT(HDRP(NEXT_BLKP(oldptr)), PACK(remain_size, 0));
                    PUT(FTRP(NEXT_BLKP(oldptr)), PACK(remain_size, 0));
                    coalesce(NEXT_BLKP(oldptr));
                } else { 
                    // we are not going to do anything because remsize < DSIZE meaning
                    // remsize = WSIZE.  
                    // but this case may be impossible under the condition of alignment of DSIZE.
                    printf("found 1 WSIZE fragment unexpected.\n");
                    exit(-1);
                }
                newptr = oldptr;

            } 
            else if (prev_free && GET_SIZE(HDRP(PREV_BLKP(oldptr)))+old_size>=asize) {
                size_t remain_size = GET_SIZE(HDRP(PREV_BLKP(oldptr)))+old_size-asize;
                char *prev_bp = PREV_BLKP(oldptr);
                PUT(HDRP(prev_bp), PACK(asize, 1));
                PUT(FTRP(prev_bp), PACK(asize, 1));
                if (remain_size >= DSIZE) {
                    PUT(HDRP(NEXT_BLKP(prev_bp)), PACK(remain_size, 0));
                    PUT(FTRP(NEXT_BLKP(prev_bp)), PACK(remain_size, 0));
                    coalesce(NEXT_BLKP(prev_bp));
                } else {
                    // same as case of next_free.
                    printf("found 1 WSIZE fragment unexpected.\n");
                    exit(-1);
                }
                memcpy(prev_bp, oldptr, copy_size-DSIZE);
                newptr = prev_bp;
            }
        } else if (asize < old_size) {
            PUT(HDRP(oldptr), PACK(asize, 1));
            PUT(FTRP(oldptr), PACK(asize, 1));
            PUT(HDRP(NEXT_BLKP(oldptr)), PACK(old_size-asize, 0));
            PUT(FTRP(NEXT_BLKP(oldptr)), PACK(old_size-asize, 0));
            coalesce(NEXT_BLKP(oldptr));
            newptr = oldptr;
        }
    } else {
        memcpy(newptr, oldptr, copy_size-DSIZE);
        mm_free(oldptr);
        // TODO Why Coalesce results in `perserve data from old` fails?
        //coalesce(oldptr);
    }
    
    return newptr;
}

//void *mm_realloc(void *ptr, size_t size)
//{
//    if(ptr == NULL){
//        return mm_malloc(size);
//    }
//    if(size == 0){
//        mm_free(ptr);
//        return NULL;
//    }
//
//    size_t asize;
//    if(size <= DSIZE) asize  = 2 * DSIZE;
//    else asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1))/DSIZE);
//
//    size_t oldsize = GET_SIZE(HDRP(ptr));
//    if(oldsize == asize) return ptr;
//    else if(oldsize > asize){
//        PUT(HDRP(ptr), PACK(asize, 1));
//        PUT(FTRP(ptr), PACK(asize, 1));
//        PUT(HDRP(NEXT_BLKP(ptr)), PACK(oldsize - asize, 0));
//        PUT(FTRP(NEXT_BLKP(ptr)), PACK(oldsize - asize, 0));
//        //immediate_coalesce(NEXT_BLKP(ptr));
//        //immediate or delay
//        return ptr;
//    }
//    else{
//        size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
//        if(!next_alloc && GET_SIZE(HDRP(NEXT_BLKP(ptr))) + oldsize >= asize) {
//            size_t last = GET_SIZE(HDRP(NEXT_BLKP(ptr))) + oldsize - asize;
//            PUT(HDRP(ptr), PACK(asize, 1));
//            PUT(FTRP(ptr), PACK(asize, 1));
//            if(last >= DSIZE){
//                PUT(HDRP(NEXT_BLKP(ptr)), PACK(last, 0));
//                PUT(FTRP(NEXT_BLKP(ptr)), PACK(last, 0));
//            }
//            return ptr;
//        }
//        else{
//            char *newptr = mm_malloc(asize);
//            if(newptr == NULL) return NULL;
//            memcpy(newptr, ptr, oldsize - DSIZE);
//            mm_free(ptr);
//            return newptr;
//        }
//    }
//}
