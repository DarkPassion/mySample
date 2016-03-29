#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "thread/thread.h"
#include "thread/lock.h"


// g++ thread.cpp -I../src/ -L../libs/ -lcommon -o thread.out
class Thread1 : public ThreadHandle
{
public:
    Thread1()
    {
        printf("thread1 construct ! \n");
        
        _pth = new ThreadImp("Thread1", this, 1000, false);
        _pth->start();
    }
    
    ~Thread1()
    {
        printf("thread1 deconstruct! \n");
        _pth->stop_loop();
        _pth->stop();
        freep(_pth);

    }
    
    
    virtual int cycle()
    {
//        while (_pth->can_loop()) {
//            
//            printf("lopp === \n");
//        }
        
        AutoLock __lock(_mutex);
        printf("thread cycle  ! \n");
        
        return -1;
    }
    
    virtual void on_thread_start()
    {
        printf("on thread start ! \n");
    }
    
    virtual void on_thread_stop()
    {
        printf("on thread stop ! \n");
    }
    
    virtual int on_before_cycle()
    {
        printf("on before cycle ! \n");
        return 0;
    }
    
    virtual int on_end_cycle()
    {
        printf("on end cycle ! \n");
        return 0;
    }
    
    
private:
    
    ThreadImp*  _pth;
    CMutex _mutex;
    
};


int main()
{
    while (1) {
        usleep(100 * 1000);
        
        Thread1* pth = new Thread1();
        
        delete pth;
    }
    
}