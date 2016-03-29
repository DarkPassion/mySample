#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "thread/thread.h"
#include "thread/lock.h"


// g++ thread.cpp -I../src/ -L../libs/ -lcommon -o thread.out

int start_threads = 0;
int stop_threads = 0;

class Thread1 : public ThreadHandle
{
public:
    Thread1()
    {
        printf("thread1 construct ! \n");
        
        _pth = new ThreadImp("Thread1", this, 1000, true);
        _pth->start();
        start_threads++;
    }
    
    ~Thread1()
    {
        printf("thread1 deconstruct! \n");
        //_pth->stop_loop();
        //_pth->stop();
        
        // this delete maybe crash!!!
        //freep(_pth);
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
        stop_threads++;
        printf("on thread stop ! [%d %d]\n", start_threads, stop_threads);
        
        // here delete
        freep(_pth);
        delete this;
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
    int max_num = 100* 10000; 
    while (1) {
        if(max_num-- < 1) {
		usleep(100 * 1000);
	} else {
		printf("main thread1 create ! \n");
        	Thread1* pth = new Thread1();

	}

	    
    }
    
}
