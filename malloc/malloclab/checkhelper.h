#ifndef CHKH_H
#define CHKH_H
static char *start_brk = NULL;
static char *final_brk = NULL;
static char *starterbp = NULL; // staring pointer at this block
static char *nxt_starterbp = NULL; // ending pointer at this block,
#include "mm.c"
extern char *heap_listp;
int mm_checker(void);

void inspectBlock(char *p);

static void *combineNext(void *bp);

void printblock(char *p, int type);

int isValidHeap();

void displayHeap(int, char*);

void pblockinfo(char *);
#endif
