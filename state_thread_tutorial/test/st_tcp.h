#ifndef _ST_TCP_H_
#define _ST_TCP_H_
#include <st.h>
#include <st_thread.h>
#include <string>


/**
* the tcp connection handler.
*/
class ISrsTcpHandler
{
public:
    ISrsTcpHandler();
    virtual ~ISrsTcpHandler();
public:
    /**
    * when got tcp client.
    */
    virtual int on_tcp_client(st_netfd_t stfd) = 0;
};


/**
* bind and listen tcp port, use handler to process the client.
*/
class SrsTcpListener : public ITthreadHandle
{
private:
    int _fd;
    st_netfd_t _stfd;
    STThread* pthread;
private:
    ISrsTcpHandler* handler;
    std::string ip;
    int port;
public:
    SrsTcpListener(ISrsTcpHandler* h, std::string i, int p);
    virtual ~SrsTcpListener();
public:
    virtual int fd();
public:
    virtual int listen();
// interface ISrsReusableThreadHandler.
public:
    virtual int cycle();
};

#endif