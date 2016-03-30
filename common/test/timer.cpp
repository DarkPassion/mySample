#include <stdlib.h>
#include <stdio.h>

#include "timer/timer.h"


// g++ timer.cpp -I../src/ -L../libs/ -lcommon -o timer.out

class MyTimer : public ITimerHandle
{

public:
    MyTimer()
    {
        _timer = new CTimer(this, 10*1000);
        
        _timer->set_timer(101, 5 * 1000);
        
        _timer->set_timer(102, 1 * 1000);
        
        _timer->start();
    }
    
    ~MyTimer()
    {
        delete _timer;
    }
    
    // ITimerHandle
    
    virtual void on_time(int id)
    {
        printf("zzf on timer [%d] \n", id);
    }
    
private:
    CTimer* _timer;
    
};


int main()
{
    MyTimer timer;
    
    while (1) {
        usleep(400 * 1000);
    }
    return 0;
}




