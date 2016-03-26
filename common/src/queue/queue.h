#ifndef _QUEUE_QUEUE_H_
#define _QUEUE_QUEUE_H_


#include <stdlib.h>
#include <vector>
#include "thread/lock.h"

template <class T>
class Queue
{
public:
    Queue();
    
    ~Queue();
    
    
    void push_back(T* val);
    
    T* pop_front();
    
    size_t size();
    
    void clear();
    
private:
    typedef std::vector<T*> TypeQueue;
    
    TypeQueue   _q;
    
    CMutex  _mutex;

};


#endif
