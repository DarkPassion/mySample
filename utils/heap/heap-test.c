
#include <stdlib.h>
#include <stdio.h>

#include "heap.h"

// https://github.com/kr/beanstalkd/blob/master/heap.c

typedef struct job_s
{
    size_t idx;
    uint32_t pri;
} job_t;

job_t* make_job(int pri);


int job_pri_less(void* ax, void* bx)
{

    job_t* aj = (job_t*) ax;
    job_t* bj = (job_t*) bx;

    return aj->pri < bj->pri;
}


void job_setheappos(void* j, int pos)
{
    job_t* jx = (job_t*)j;
    jx->idx = pos;
}

job_t * make_job(int pri)
{
    job_t * t = (job_t*) malloc(sizeof(job_t));
    memset(t, 0, sizeof(job_t));
    t->pri = pri;
    return t;
}


void test_heap_func1();
void test_heap_func2();


void test_heap_func1()
{
    job_t* j1 = make_job(1);
    job_t* j2 = make_job(2);

    Heap h = {0};

    h.less = job_pri_less;
    h.rec = job_setheappos;

    heapinsert(&h, j1);
    if (h.len == 1) {
        printf("heap test heap.len success! \n");
    } else {
        printf("heap test heap.len fail! \n");
    }
    heapinsert(&h, j2);
    if (h.len == 2) {
        printf("heap test heap.len success! \n");
    } else {
        printf("heap test heap.len fail! \n");
    }


    printf("heap index [%zu %zu] \n", j1->idx, j2->idx);



    job_t*j = heapremove(&h, 0);
    if (h.len == 1) {
        printf("heap test heap.len success! \n");
    } else {
        printf("heap test heap.len fail ! \n");
    }

    if (j == j1) {
        printf("heap test heapmove success ! \n");
    }

    j = heapremove(&h, 0);
    if (j == j2) {
        printf("heap test heapmove success ! \n");
    }

    free(j1);
    free(j2);
}


void test_heap_func2()
{
    Heap h = {0};

    h.less = job_pri_less;
    h.rec = job_setheappos;

    printf("test heap func2 == \n");
    job_t* arr[10] = {0};
    for (int i = 0; i < 10; i++) {
        job_t* j = make_job(10-i);
        arr[i] = j;
        heapinsert(&h, j);
    }

    printf("heap len = %d \n", h.len);

    for (int i = 0; i < 10; i++) {
        printf("job_t %d index %zu pri %d\n", i, arr[i]->idx, arr[i]->pri);
    }

    while(h.len > 0) {
        job_t * j = heapremove(&h, 0);
        printf("job_t will remove index %zu %d\n", j->idx, j->pri);
        free(j);
    }
}



int main()
{
    printf("==== \n");
    test_heap_func1();
    test_heap_func2();

    return 0;
}


