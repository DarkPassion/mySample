#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "thread.h"

enum ThreadStatus
{
    THREAD_INIT = 1,
    THREAD_RUNING,
    THREAD_STOP
};

ThreadHandle::ThreadHandle()
{

}

ThreadHandle::~ThreadHandle()
{

}

void ThreadHandle::on_thread_start()
{

}


void ThreadHandle::on_thread_stop()
{

}


int ThreadHandle::on_before_cycle()
{
    return 0;
}


int ThreadHandle::on_end_cycle()
{
    return 0;
}

ThreadImp::ThreadImp(const char* name, ThreadHandle* handle, int interval_ms, int joinable)
{
    printf("ThreadImp [%s %p %d %d]\n", name, handle, interval_ms, joinable);
    _name = name;
    _handle = handle;
    _interval_ms = interval_ms;

    _loop = 0;
    _pth = 0;
    _joinable = joinable;
    _status = THREAD_INIT;

}

ThreadImp::~ThreadImp()
{
    stop();
}

int ThreadImp::start()
{
    if (_pth)
    {
        fprintf(stdout, "thread already run ! \n");
        return 0;
    }

    _loop = 1;
    pthread_create(&_pth, NULL, thread_func, this);
    return 0;
}

int ThreadImp::stop()
{
    if (_status == THREAD_INIT)
    {
        return 0;
    }
    _loop = 0;


    if (_joinable && _pth)
    {
        pthread_join(_pth, NULL);
    }
    _status = THREAD_INIT;

    return 0;
}

void ThreadImp::stop_loop()
{
    _loop = 0;
}


void* ThreadImp::thread_func(void* param)
{
    ThreadImp* pthis = (ThreadImp*)param;
    pthis->thread_cycle();
    return NULL;
}


void ThreadImp::thread_cycle()
{

    assert(_handle);
    _handle->on_thread_start();

    _status = THREAD_RUNING;
    while (_loop)
    {

        if (_handle->on_before_cycle() != 0)
        {
            break;
        }

        if (_handle->cycle() != 0)
        {
            break;
        }

        if (_handle->on_end_cycle() != 0)
        {
            break;
        }

        if (_interval_ms)
        {
            usleep(_interval_ms * 1000);
        }

    }

    _handle->on_thread_stop();
    _status = THREAD_STOP;

}

int ThreadImp::can_loop()
{
    return _loop;
}









