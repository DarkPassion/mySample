#ifndef _THREAD_LOCK_H_
#define _THREAD_LOCK_H_

#include <pthread.h>
#include "util/util.h"

class ILock
{
public:
    virtual ~ILock() {};

    virtual void lock() = 0;

    virtual void unlock() = 0;
};

class CMutex;
class CCond
{
public:
    CCond(CMutex& mutex);
    
    ~CCond();
    
    void notify();
    
    void notify_all();
    
    void wait();
    
    DISALLOW_COPY_AND_ASSIGN(CCond);
    
private:
    CMutex&     _lock;
    pthread_cond_t  _cond;
};

class CMutex : public ILock
{

public:
    CMutex();

    virtual ~CMutex();

    virtual void lock();

    virtual void unlock();

    DISALLOW_COPY_AND_ASSIGN(CMutex);
    
    friend class CCond;
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
