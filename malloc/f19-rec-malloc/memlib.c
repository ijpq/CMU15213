/*
 * memlib.c - a module that simulates the memory system.  Needed
 * because it allows us to interleave calls from the student's malloc
 * package with the system's malloc package in libc.
 *
 * This version has been updated to enable sparse emulation of very large heaps
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include "memlib.h"
#include "config.h"

/* Data structure used to implement pages in sparse memory emulation */
typedef struct MBLK {
    size_t id;                             /* Page ID.  Counts number of pages from start of heap */
    struct MBLK *next;                     /* Link for hash table */
    unsigned char bytes[SPARSE_PAGE_SIZE]; /* Page contents */
} mem_block_t;

/* private global variables */
static bool sparse = false;                 /* Use sparse memory emulation */
static unsigned char *heap;                 /* Starting address of heap */
static unsigned char *mem_brk;              /* Current position of break */
static unsigned char *mem_max_addr;         /* Maximum allowable heap address */
static size_t mmap_length = MAX_DENSE_HEAP; /* Number of bytes allocated by mmap */
static bool show_stats = false;             /* Should program print allocation information? */
static bool stats_printed = false;          /* Has information been printed about allocation */

/* Sparse memory representation */
static mem_block_t *next_free_page = NULL;  /* Next free page */
static size_t num_pages = 0;                /* Total number of pages */
static size_t num_free_pages = 0;           /* Number of free pages */
static mem_block_t **page_table = NULL;     /* Hash table from page ID to page */
static size_t num_buckets = 0;              /* Number of buckets in page table */

/*
 * Forward declarations
 */
static size_t page_id(const void *addr);
static void *page_start(size_t id);
static void *get_mem(const void *addr);
static void print_stats();

/* 
 * mem_init - initialize the memory system model
 */
void mem_init(bool do_sparse){
    sparse = do_sparse;
    if (sparse) {
        /* Want sparse total allocation to approximately match the dense heap size */
        /* Account for both page itself and its amortized contribution to the page table */
        double fbytes_per_page = sizeof(mem_block_t) + sizeof(mem_block_t *) / HASH_LOAD;
        num_pages = (size_t) (MAX_DENSE_HEAP / fbytes_per_page);
        num_buckets = num_pages / HASH_LOAD;
        mmap_length =
            num_buckets * sizeof(mem_block_t *) +  // Page table
            num_pages * sizeof(mem_block_t) +      // Pages
            sizeof(uint64_t);                      // Padding
    } else {
        /* Dense allocation */
        next_free_page = NULL;
        num_pages = 0;
        page_table = NULL;
        num_buckets = 0;
        mmap_length = MAX_DENSE_HEAP;
    }

    int dev_zero = open("/dev/zero", O_RDWR);
    void *start = sparse ? NULL : TRY_DENSE_HEAP_START;
    void *addr = mmap(start,        /* suggested start*/
                      mmap_length,  /* length */
                      PROT_WRITE,   /* permissions */
                      MAP_PRIVATE,  /* private or shared? */
                      dev_zero,            /* fd */
                      0);            /* offset */
    if (addr == MAP_FAILED) {
        fprintf(stderr, "FAILURE.  mmap couldn't allocate space for heap\n");
        exit(1);
    }
    if (sparse) {
        /* Use initial space for page table */
        page_table = (mem_block_t **) addr;
        heap = SPARSE_HEAP_START;
        mem_max_addr = heap + MAX_SPARSE_HEAP;
    } else {
        heap = addr;
        mem_max_addr = heap + MAX_DENSE_HEAP;
    }
    stats_printed = false;
    mem_brk = heap;
    mem_reset_brk();
}

/* 
 * mem_deinit - free the storage used by the memory system model
 */
void mem_deinit(void){
    print_stats();
    munmap(heap, mmap_length);
    next_free_page = NULL;
    num_free_pages = 0;
    page_table = NULL;
    num_buckets = 0;
}

/*
 * mem_reset_brk - reset the simulated brk pointer to make an empty heap
 */
void mem_reset_brk(){
    print_stats();
    if (sparse) {
        /* Clear page table */
        size_t ptb = num_buckets * sizeof(mem_block_t *);
        memset((void *) page_table, 0, ptb);
        /* First page is just beyond page table */
        next_free_page = (mem_block_t *) ((unsigned char *) page_table + ptb);
        num_free_pages = num_pages;
    }
    mem_brk = heap;
}

/* 
 * mem_sbrk - simple model of the sbrk function. Extends the heap 
 *                by incr bytes and returns the start address of the new area. In
 *                this model, the heap cannot be shrunk.
 */
void *mem_sbrk(intptr_t incr) {
    unsigned char *old_brk = mem_brk;

    bool ok = true;
    if (incr < 0) {
        ok = false;
        fprintf(stderr, "ERROR: mem_sbrk failed.  Attempt to expand heap by negative value %ld\n", (long) incr);
    } else if (mem_brk + incr > mem_max_addr) {
        ok = false;
        size_t alloc = mem_brk - heap + incr;
        fprintf(stderr, "ERROR: mem_sbrk failed. Ran out of memory.  Would require heap size of %zd (0x%zx) bytes\n", alloc, alloc);
    } else if (!sparse && sbrk(incr) == (void*) -1) {
        ok = false;
        fprintf(stderr, "ERROR: mem_sbrk failed.  Could not allocate more heap space\n");
    }
    if (ok) {
        mem_brk += incr;
        return (void *) old_brk;
    } else {
        errno = ENOMEM;
        return (void *) -1;
    }
}

/*
 * mem_heap_lo - return address of the first heap byte
 */
void *mem_heap_lo(){
    return (void *) heap;
}

/* 
 * mem_heap_hi - return address of last heap byte
 */
void *mem_heap_hi(){
    return (void *)(mem_brk - 1);
}

/*
 * mem_heapsize() - returns the heap size in bytes
 */
size_t mem_heapsize() {
    return (size_t)(mem_brk - heap);
}

/*
 * mem_pagesize() - returns the page size of the system
 */
size_t mem_pagesize(){
    return (size_t) getpagesize();
}

/*************** Memory emulation  *******************/

__int128 mem_read128(const void* addr)
{
    __int128 r;
    r = (((__int128)mem_read((char*)addr + 8, 8))<<64) | mem_read(addr, 8);
    
    return r;
}

void mem_write128(void* addr, __int128 val)
{
    mem_write(addr, (uint64_t)val, 8);
    mem_write((char*)addr + 8, (uint64_t)(val >> 64), 8);
}

/* Read len bytes and return value zero-extended to 64 bits */
uint64_t mem_read(const void *addr, size_t len) {
    uint64_t rdata;
    if (sparse &&
        (unsigned char *) addr >= heap && (unsigned char *) addr+len <= mem_brk) {
        /* Heap read.  Check if it crosses page boundary */
        size_t id = page_id(addr);
        void *paddr = get_mem(addr);
        rdata =  *(uint64_t *) paddr;
        /* Check for split pages */
        void *maddr = (void *) ((unsigned char *) addr + len - 1);
        if (id != page_id(maddr)) {
            void *saddr = page_start(id);
            size_t offset = (unsigned char *) addr - (unsigned char *) saddr;
            size_t llen = SPARSE_PAGE_SIZE - offset;
            /* Must zero out upper bytes of data */
            uint64_t mask = ((uint64_t) 1 << (8 * llen)) - 1;
            rdata &= mask;
            void *haddr = (void *) ((unsigned char *) addr + llen);
            void *hpaddr = get_mem(haddr);
            uint64_t hdata = *(uint64_t *) hpaddr;
            rdata = rdata | (hdata << (8 * llen));
        }
    } else {
        /* Dense or non-heap read */
        rdata = *(uint64_t *) addr;
    }
    if (len < sizeof(uint64_t)) {
        uint64_t mask = ((uint64_t) 1 << (8 * len)) - 1;
        rdata &= mask;
    }
    return rdata;
}

/* Write lower order len bytes of val to address */
void mem_write(void *addr, uint64_t val, size_t len) {
    if (sparse &&
        (unsigned char *) addr >= heap && (unsigned char *) addr+len <= mem_brk) {
        /* Heap write.  Check to see if it crosses page boundary */
        size_t id = page_id(addr);
        void *paddr = get_mem(addr);
        void *saddr = page_start(id);
        size_t offset = (unsigned char *) addr - (unsigned char *) saddr;
        size_t llen = SPARSE_PAGE_SIZE - offset;
        if (llen < len) {
            /* Two page write */
            memcpy(paddr, (void *) &val, llen);
            size_t ulen = len - llen;
            void *haddr = (void *) ((unsigned char *) addr + llen);
            void *hpaddr = get_mem(haddr);
            unsigned char *src = (unsigned char *) &val + llen;
            memcpy(hpaddr, (void *) src, ulen);
        } else {
            /* Single page write */
            if (len == sizeof(uint64_t))
                *(uint64_t *) paddr = val;
            else
                memcpy(paddr, (void *) &val, len);
        }
    } else {
        /* Dense or non-heap write */
        if (len == sizeof(uint64_t))
            *(uint64_t *) addr = val;
        else
            memcpy(addr, (void *) &val, len);
    }
}

/* Emulation of memcpy */
void *mem_memcpy(void *dst, const void *src, size_t n) {
    void *savedst = dst;
    size_t w = sizeof(uint64_t);
    while (n >= w) {
        uint64_t data = mem_read(src, w);
        mem_write(dst, data, w);
        n -= w;
        src = (void *) ((unsigned char *) src + w);
        dst = (void *) ((unsigned char *) dst + w);
    }
    if (n) {
        uint64_t data = mem_read(src, n);
        mem_write(dst, data, n);
    }
    return savedst;
}

/* Emulation of memset */
void *mem_memset(void *dst, int c, size_t n) {
    void *savedst = dst;
    uint64_t byte = c & 0xFF;
    uint64_t data = 0;
    size_t w = sizeof(uint64_t);
    size_t i;
    for (i = 0; i < w; i++) {
        data = data | (byte << (8*i));
    }
    while (n >= w) {
        mem_write(dst, data, w);
        n -= w;
        dst = (void *) ((unsigned char *) dst + w);
    }
    if (n) {
        mem_write(dst, data, n);        
    }
    return savedst;
}

/* Function to aid in viewing contents of heap */
void hprobe(void *ptr, int offset, size_t count) {
    unsigned char *cptr = (unsigned char *) ptr;
    unsigned char *cptr_lo = cptr+offset;
    unsigned char *cptr_hi = cptr_lo + count - 1;
    unsigned char *iptr;
    if ((void *) cptr_lo < mem_heap_lo()) {
        fprintf(stderr, "Invalid probe.  Address %p is below start of heap\n",
                cptr_lo);
        return;
    }
    if ((void *) cptr_hi > mem_heap_hi()) {
        fprintf(stderr, "Invalid probe.  Address %p is beyond end of heap\n",
                cptr_lo);
        return;
    }
    printf("Bytes %p...%p: 0x", cptr_hi, cptr_lo);
    for (iptr = cptr_hi; iptr >= cptr_lo; iptr--)
        printf("%.2x", (unsigned) mem_read((void *) iptr, 1));
    printf("\n");
}


/*************** Private Functions *******************/


static void print_stats() {
    size_t vbytes = mem_heapsize();
    if (!show_stats || vbytes == 0 || stats_printed)
        return;
    if (sparse) {
        size_t ppages = num_pages - num_free_pages;
        size_t pbytes = ppages * SPARSE_PAGE_SIZE;
        printf("Allocated %zu/%zu pages (%zu bytes) to cover %zu heap bytes (%.4f%% density).  Max address = %p\n",
               ppages, num_pages, pbytes, vbytes, 100.0 * pbytes / vbytes, mem_brk);
    } else {
        printf("Allocated %zu heap bytes.  Max address = %p\n",
               vbytes, mem_brk);
    }
    stats_printed = true;
}

/* Given an address, compute the ID  of its page */
static size_t page_id(const void *addr) {
    size_t offset = (unsigned char *) addr - (unsigned char *) SPARSE_HEAP_START;
    return offset / SPARSE_PAGE_SIZE;
}

/* Given a page ID, compute its starting address */
static void *page_start(size_t id) {
    size_t offset = id * SPARSE_PAGE_SIZE;
    return (void *) ((unsigned char *) SPARSE_HEAP_START + offset);
}

/* Get memory to store value.  Allocate page if necessary */
static void *get_mem(const void *addr) {
    size_t id = page_id(addr);
    size_t b = id % num_buckets; // A very simple hash function
    mem_block_t *block = page_table[b];
    while (block && block->id != id)
        block = block->next;
    if (!block) {
        /* Need to allocate a new block */
        if (num_free_pages == 0) {
            fprintf(stderr, "FAILURE.  Ran out of memory for emulation\n");
            exit(1);
        }
        block = next_free_page++;
        num_free_pages--;
        block->id = id;
        block->next = page_table[b];
        page_table[b] = block;
    }
    void *saddr = page_start(id);
    size_t offset = (unsigned char *) addr - (unsigned char *) saddr;
    return (void *) &block->bytes[offset];
}

