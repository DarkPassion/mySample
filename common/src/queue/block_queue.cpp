
#include "util/util.h"
#include "block_queue.h"

template<class T>
BlockQueue::BlockQueue()
{
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond, NULL);
}

template <class T>
BlockQueue::~BlockQueue()
{
    clear();
    pthread_cond_destroy(&_cond);
    pthread_mutex_destroy(&_mutex);
}


template <class T>
void BlockQueue::push_back(T* val)
{
    pthread_mutex_lock(&_mutex);
    
    _q.push_back(val);
    
    pthread_cond_signal(&_cond);
    pthread_mutex_unlock(&_mutex);
    
}

template<class T>
T* BlockQueue::pop_front()
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


template<class T>
void BlockQueue::clear()
{
    pthread_mutex_lock(&_mutex);
    
    while (_q.size()) {
        
        T* val = _q.front();
        _q.erase(_q.begin());
        freep(val);
        
    }
    
    pthread_mutex_unlock(&_mutex);

}

template <class T>
size_t BlockQueue::size()
{
    size_t s = 0;
    pthread_mutex_lock(&_mutex);

    s = _q.size();
    
    pthread_mutex_unlock(&_mutex);

    return s;
}


