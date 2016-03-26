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


AutoLock::AutoLock(ILock* lock)
{
    _lock = lock;
    _lock->lock();
}

AutoLock::~AutoLock()
{
    _lock->unlock();
}