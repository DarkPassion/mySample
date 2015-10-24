#include <st_thread.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

STTHreadContext*  _stThreadCtx = new STTHreadContext();


ITthreadHandle::ITthreadHandle()
{

}

ITthreadHandle::~ITthreadHandle()
{

}

int ITthreadHandle::before_cycle()
{
    return 0;
}

int ITthreadHandle::end_cycle()
{
    return 0;
}

void ITthreadHandle::on_thread_start()
{

}

void ITthreadHandle::on_thread_stop()
{

}


STThread::STThread(const char* name, ITthreadHandle* handle,  int64_t interval_us,  bool joinable)
{
    _name = name;
    _handle = handle;
    _interval_us = interval_us;
    _joinable = joinable;

    tid = NULL;
    loop = false;
    _canrun = false;
    _cid = -1;
}

STThread::~STThread()
{
    stop();
}


int STThread::start()
{
    int ret = 0;
    if (tid) {
        fprintf(stderr, "STThread already start!\n");
        return ret;
    }

    if (tid = st_thread_create(thread_fun,  this, (_joinable ? 1 : 0), 0), tid == NULL) {
        fprintf(stderr, "st_thread_create fail\n");
        return -1;
    }

    loop = true;

    while (_cid < 0 && loop) {
        st_usleep(10 * 1000);
    }

    _canrun = true;

    return ret;
}



int STThread::stop()
{
    if (tid) {
        loop = false;

        st_thread_interrupt(tid);

        if (_joinable) {
            int ret = st_thread_join(tid, NULL); 
            if (ret) {
            }
        }

        st_usleep(10 * 1000);

        tid = NULL;
    }
    return 0;
}


bool STThread::can_loop()
{
    return loop;
}


void STThread::stop_loop()
{
    loop = false;
}


void STThread::thread_cycle()
{

    int ret = 0;
    _stThreadCtx->generate_id();

    _cid = _stThreadCtx->get_id();

    _handle->on_thread_start();

    while(!_canrun && loop) {
        st_usleep(10 * 1000);
    }


    while (loop) {
        if ((ret = _handle->before_cycle()) != 0) {
            goto fail;
        }

        if (ret = _handle->cycle() , ret!= 0) {
            goto fail;
        }

        if ((ret = _handle->end_cycle()) != 0) {
            goto fail;
        }
 fail:
    if (!loop) {
        break; 
    }

   
    }

    _handle->on_thread_stop();

}


void * STThread::thread_fun(void * arg)
{
    STThread* st = (STThread*)arg;
    assert(st);
    st->thread_cycle();
    return NULL;

}


STTHreadContext::STTHreadContext()
{

}

STTHreadContext::~STTHreadContext()
{

}

int STTHreadContext::generate_id()
{
    static int id = 100;
    int gid = id++;

    cache[st_thread_self()] = gid;
    return gid;
}

int STTHreadContext::get_id()
{

    return cache[st_thread_self()];
}
