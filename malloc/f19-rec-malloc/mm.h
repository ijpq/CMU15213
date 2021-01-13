#include <stdio.h>
#include <stdbool.h>

#ifdef DRIVER

/* declare functions for driver tests */
extern void *mm_malloc (size_t size);
extern void mm_free (void *ptr);
extern void *mm_realloc(void *ptr, size_t size);
extern void *mm_calloc (size_t nmemb, size_t size);

#else

/* declare functions for interpositioning */
extern void *malloc (size_t size);
extern void free (void *ptr);
extern void *realloc(void *ptr, size_t size);
extern void *calloc (size_t nmemb, size_t size);

#endif

extern bool mm_init(void);

/* This is for debugging.  Returns false if error encountered */
extern bool mm_checkheap(int lineno);
