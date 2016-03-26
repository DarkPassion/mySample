#ifndef _QUEUE_BLOCK_QUEUE_
#define _QUEUE_BLOCK_QUEUE_

#include <stdlib.h>
#include <vector>
#include <pthread.h>
#include "util/util.h"

template <class T>
class BlockQueue
{
public:
    BlockQueue();
    
    ~BlockQueue();
    
    void push_back(T* val);
    
    T* pop_front();
    
    void clear();
    
    size_t size();
    
private:
    
    typedef std::vector<T*> TypeQueue;
    TypeQueue       _q;
    
    pthread_mutex_t _mutex;
    pthread_cond_t  _cond;
    
};



#endif



