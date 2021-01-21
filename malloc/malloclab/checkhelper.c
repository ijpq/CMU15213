#include <stdio.h>
#include <stdlib.h>
#include "macro.h"
#include "checkhelper.h"

//extern char *heap_listp;
//extern char *start_brk;
//extern char *final_brk;
//extern char *starterbp;
//extern char *nxt_starterbp;

int mm_checker(void) {
    return isValidHeap();   
}
void inspectBlock(char *p) {
    printf("error word at : %p\n", p);
    printf("%x\t%x\t%x\t%x\n",*p, *(p+1), *(p+2), *(p+3));
}
static void *combineNext(void *bp) {
    // you should assure next block is freed before this function invoked.
}
void printblock(char *p, int type) {
    if (type == 0) { // HDR/FTR/PRO/EPI
        int size = GET_SIZE(p);
        int alloc = IS_ALLOC(p);
        printf("------\n");
        printf("|%d/%d|\n", size, alloc);
        printf("------\n");
    } else if (type == 1) {
        if (GET(p) != 0) {
            printf("------\n");
            printf("|%d,%d,%d,%d|\n", *p, *(p+1), *(p+2), *(p+2));
            printf("------\n");
        } else {
            printf("------\n");
            printf("|    |\n");
            printf("------\n");
        }
    }
}

void pblockinfo(char *p) {
    printf("this block starting at %p\n", p);
    printf("this block ending at %p\n", p+GET_SIZE(p));
    unsigned int size = GET_SIZE(p);
    printf("this block length is %u, 0x%x\n", size, size);
    return;
}

void displayHeap(int len, char *p) {
    printf("first ptr %p\n", start_brk);
    printf("final ptr %p\n", final_brk);
    char *ptr = HDRP(NEXT_BLKP(heap_listp));
    char c;
    int blockcnt = 1;
    while (ptr < final_brk) {
        printf("input ENTER to continue\n");
        c = getchar();
        if (c == '\n') {
            printf("blocid : %d\n", blockcnt++);
            if (ptr == starterbp) {
                //printf("enter error block\n");
                printf("enter current block\n");
                pblockinfo(ptr);
                inspectBlock(p);
            } else {
                pblockinfo(ptr);
            }
            if (GET_SIZE(ptr) == 0) {
                printf("reach epilogue\n");
                return 0;
            }
            ptr = ptr + GET_SIZE(ptr);
        } else {
            continue;
        }
    }
}
int isValidHeap() {
    /*
     * prologue/epilogue block
     */
    char *heaPtr = heap_listp;
    if (GET_SIZE(HDRP(heaPtr)) != 8 || IS_ALLOC(HDRP(heaPtr)) != 1) {
        printf("error first prologue block\n");
        return 0;
    }
    if (GET_SIZE(heaPtr) != 8 || IS_ALLOC(heaPtr) != 1) {
        printf("error second prologue block\n");
        return 0;
    }

    if (GET_SIZE(HDRP(final_brk)) != 0 || !IS_ALLOC(HDRP(final_brk))) {
        printf("error epilogue block\n");
        return 0;
    }

    /*
     * continuous free block
     */
    heaPtr = heap_listp;
    int FreeBefore = 0;
    while (heaPtr != final_brk) {
        if (!IS_ALLOC(HDRP(heaPtr))) {
            if (FreeBefore) {
                printf("two free block adjcent, but not coalsce\n");
                return 0;
            }
            FreeBefore = 1;
        } else if(IS_ALLOC(HDRP(heaPtr))) {
                FreeBefore = 0;
        }
        heaPtr = NEXT_BLKP(heaPtr);
    }

    /*
     * overlap
     */
    heaPtr = NEXT_BLKP(heap_listp) - WSIZE;
    int beforeHeader = 1;
    int length = 0;
    int alength = 0;
    int isalloc = 0;
    int error = 0;
    while (heaPtr != final_brk) {
        if (beforeHeader) { // at bp
            starterbp = heaPtr;
            length = GET_SIZE(heaPtr);
            nxt_starterbp = starterbp + length;
            alength = 0;
            beforeHeader = 0;
            isalloc = IS_ALLOC(heaPtr);
        } else if (alength < length - WSIZE) { // at normal
            // this checking is deleted \
            // because we cannot gurantee all-zero value in block between HDR and FTR.
            
            //if (GET(heaPtr) != 0) {
            //    printf("non zero value in normal block\n");
            //    error = 1;
            //    break;
            //}
        } else if (alength == length - WSIZE){ // at FTR
            if (GET_SIZE(heaPtr) != length) {
                printf("HDR size %d != FTR size %d \n", length, GET_SIZE(heaPtr));
                error = 1;
                break;
            }
            if (IS_ALLOC(heaPtr) != isalloc) {
                printf("HDR alloc != FTR alloc\n");
                error = 1;
                break;
            }
            beforeHeader = 1;
        } 
        heaPtr = heaPtr + WSIZE;
        alength += WSIZE;
    }
    if (error) {
        displayHeap(alength, heaPtr);
    }
    printf("pass all check\n");
    return 1;
}
