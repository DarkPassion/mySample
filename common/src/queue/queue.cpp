

#include "queue.h"
#include "util/util.h"

template<class T>
Queue<T>::Queue()
{

}


template<class T>
Queue<T>::~Queue()
{
    clear();
}


template<class T>
void Queue<T>::clear()
{
    AutoLock __lock(_mutex);
    
    while (_q.size()) {
        
        T* val = _q.front();
        freep(val);
        _q.erase(_q.begin());
    }
    
}


template<class T>
void Queue<T>::push_back(T* val)
{
    AutoLock __lock(_mutex);
    _q.push_back(val);
}

template<class T>
T* Queue<T>::pop_front()
{
    AutoLock __lock(_mutex);
    T* val = NULL;
    if (_q.size()) {
        val = _q.front();
        _q.erase(_q.being());
    }
    
    return val;
}

template<class T>
size_t Queue<T>::size()
{
    AutoLock __lock(_mutex);
    return _q.size();
}



