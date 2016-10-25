#include "lock.h"

CMutex::CMutex()
{
    pthread_mutex_init(&_mutex, NULL);
}

CMutex::~CMutex()
{
    pthread_mutex_destroy(&_mutex);
}

void CMutex::lock()
{
    pthread_mutex_lock(&_mutex);
}

void CMutex::unlock()
{
    pthread_mutex_unlock(&_mutex);
}


CCond::CCond(CMutex& lock) : _lock(lock)
{
    pthread_cond_init(&_cond, NULL);
}

CCond::~CCond()
{
    pthread_cond_destroy(&_cond);
}

void CCond::notify()
{
    pthread_cond_signal(&_cond);
}

void CCond::notify_all()
{
    pthread_cond_broadcast(&_cond);
}

void CCond::wait()
{
    pthread_cond_wait(&_cond, &(_lock._mutex));
}

AutoLock::AutoLock(ILock& lock) : _lock(lock)
{
    _lock.lock();
}

AutoLock::~AutoLock()
{
    _lock.unlock();
}
