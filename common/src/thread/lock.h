#ifndef _THREAD_LOCK_H_
#define _THREAD_LOCK_H_

#include <pthread.h>
#include "util/util.h"

class ILock
{
public:
    virtual ~ILock(){};
    
    virtual void lock() = 0;
    
    virtual void unlock() = 0;
    
};



class CMutex : public ILock
{

public:
    CMutex();

    ~CMutex();
    
    virtual void lock();
    
    virtual void unlock();
    
    disable_default_copy(CMutex);
private:
    pthread_mutex_t _mutex;
};



class AutoLock
{

public:
    AutoLock(ILock& mutex);
    
    ~AutoLock();
    
private:
    ILock&     _lock;

};

#endif