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
    BlockQueue()
    {
        pthread_mutex_init(&_mutex, NULL);
        pthread_cond_init(&_cond, NULL);

    }
    
    ~BlockQueue()
    {
        clear();
        pthread_cond_destroy(&_cond);
        pthread_mutex_destroy(&_mutex);

    }
    
    void push_back(T* val)
    {
        pthread_mutex_lock(&_mutex);
        
        _q.push_back(val);
        
        pthread_cond_signal(&_cond);
        pthread_mutex_unlock(&_mutex);

    
    }
    
    T* pop_front()
    {
        pthread_mutex_lock(&_mutex);
        T* val = NULL;
        while (_q.size() == 0) {
            pthread_cond_wait(&_cond, &_mutex);
        }
        
        if (_q.size()) {
            val = _q.front();
            _q.erase(_q.begin());
        }
        
        pthread_mutex_unlock(&_mutex);
        
        return val;

    }
    
    void clear()
    {
        pthread_mutex_lock(&_mutex);
        
        while (_q.size()) {
            
            T* val = _q.front();
            _q.erase(_q.begin());
            freep(val);
            
        }
        
        pthread_mutex_unlock(&_mutex);
    }
    
    size_t size()
    {
        size_t s = 0;
        pthread_mutex_lock(&_mutex);
        
        s = _q.size();
        
        pthread_mutex_unlock(&_mutex);
        
        return s;
    }
    
private:
    
    typedef std::vector<T*> TypeQueue;
    TypeQueue       _q;
    
    pthread_mutex_t _mutex;
    pthread_cond_t  _cond;
    
};



#endif



