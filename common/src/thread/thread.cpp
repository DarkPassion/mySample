#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "thread.h"


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
    _name = name;
    _handle = handle;
    _interval_ms = interval_ms;

    _loop = 0;
    _pth = 0;
    _joinable = joinable;

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
    _loop = 0;


    if (_joinable && _pth)
    {
        pthread_join(_pth, NULL);
    }
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

}

int ThreadImp::can_loop()
{
    return _loop;
}









