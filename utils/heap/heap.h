#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


typedef int(*Less)(void*, void*);
typedef void(*Record)(void*, int);

struct Heap {
    int     cap;
    int     len;
    void    **data;
    Less    less;
    Record  rec;
};
typedef struct Heap Heap;

int   heapinsert(Heap *h, void *x);
void* heapremove(Heap *h, int k);


#endif

