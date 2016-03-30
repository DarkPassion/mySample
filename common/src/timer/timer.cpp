#include <algorithm>
#include <assert.h>
#include <stdint.h>
#include "timer.h"
#include "util/log.h"


struct stimer_t_0
{
    int id;
    int elapse;
    uint64_t last_timestamp;
};

// ITimerHandle
//

ITimerHandle::ITimerHandle()
{

}

ITimerHandle::~ITimerHandle()
{

}




// CTimer
//

CTimer::~CTimer()
{
    _pth->stop();
    clean_queue();
}


CTimer::CTimer(ITimerHandle* handle, int precision)
{
    _handle = handle;
    _precision = precision;

    _pth = new ThreadImp("CTimer", this, 0, true);
}

void CTimer::start()
{
    assert(_pth);
    _pth->start();

}


int CTimer::cycle()
{
    while (_pth->can_loop())
    {

        get_ticket();

        if (_precision)
        {
            usleep(_precision);
        }
    }
    return 0;
}

void CTimer::on_thread_stop()
{
    freep(_pth);
    LOGD("CTimer on_thread_stop ");
}

void CTimer::get_ticket()
{


}

void CTimer::clean_queue()
{
    AutoLock __lock(_mutex);
    while (_queue.size())
    {
        stimer_t_0* t = _queue.at(0);
        freep(t);
        _queue.erase(_queue.begin());
    }
}


void CTimer::set_timer(int id, int elapse)
{
    AutoLock __lock(_mutex);

    LOGD("CTimer set_timer [%d %d]", id, elapse);
    int bfind = 0;
    for (TypeQueue::iterator iter = _queue.begin(); iter != _queue.end(); iter++)
    {
        stimer_t_0* t = *iter;

        if (t->id == id)
        {
            // find old timer
            t->elapse = elapse;
            bfind = 1;
            break;
        }
    }

    // new timer
    if (!bfind)
    {

        stimer_t_0* t = new stimer_t_0;
        t->id = id;
        t->elapse = elapse;
        _queue.push_back(t);
    }

    // sort queue by elapse
    sort_queue();
}


void CTimer::kill_timer(int id)
{
    AutoLock __lock(_mutex);

    for (TypeQueue::iterator iter = _queue.begin(); iter != _queue.end(); iter++)
    {
        stimer_t_0* t = *iter;
        if (t->id == id)
        {
            _queue.erase(iter);
            freep(t);
            break;
        }
    }
}

bool CTimer::sort_func(stimer_t_0* ltimer, stimer_t_0* rtimer)
{
    return ltimer->elapse < rtimer->elapse;
}

void CTimer::sort_queue()
{
    std::sort(_queue.begin(), _queue.end(), CTimer::sort_func);
}

