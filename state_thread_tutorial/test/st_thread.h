#ifndef _ST_THREAD_H_
#define _ST_THREAD_H_

#include <st.h>
#include <map>


class ITthreadHandle
{
    public:
            ITthreadHandle();
            virtual ~ITthreadHandle();
            virtual int before_cycle();
            virtual int cycle() = 0;
            virtual int end_cycle();
            virtual void on_thread_start();
            virtual void on_thread_stop();
            
};


class STThread
{
    private:
            st_thread_t tid;
            bool        loop;
            const char* _name;
            ITthreadHandle* _handle;
            int64_t     _interval_us;
            bool        _joinable;
            bool        _canrun;
            int         _cid;
    public:
            STThread(const char* name, ITthreadHandle* handle,  int64_t interval_us,  bool joinable);
            ~STThread();


    public:
           virtual int start();

           virtual int stop();

    public:
           virtual bool can_loop();

           virtual void stop_loop();


    private:
            virtual void thread_cycle();
            static void* thread_fun(void* arg);
    


};

class STTHreadContext
{

public:
        STTHreadContext();

        virtual ~STTHreadContext();

        virtual int generate_id();

        virtual int get_id();

private:
        std::map<st_thread_t, int> cache;
};

extern STTHreadContext*  _stThreadCtx;


#endif