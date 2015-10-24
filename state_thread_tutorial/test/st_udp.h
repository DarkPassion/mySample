#ifndef _ST_UDP_H_
#define _ST_UDP_H_

#include <st_thread.h>
#include <st.h>
#include <string>

struct sockaddr_in;

/**
* the udp packet handler.
*/
class ISrsUdpHandler
{
public:
    ISrsUdpHandler();
    virtual ~ISrsUdpHandler();
public:
    /**
     * when fd changed, for instance, reload the listen port,
     * notify the handler and user can do something.
     */
    virtual int on_stfd_change(st_netfd_t fd);
public:
    /**
    * when udp listener got a udp packet, notice server to process it.
    * @param type, the client type, used to create concrete connection,
    *       for instance RTMP connection to serve client.
    * @param from, the udp packet from address.
    * @param buf, the udp packet bytes, user should copy if need to use.
    * @param nb_buf, the size of udp packet bytes.
    * @remark user should never use the buf, for it's a shared memory bytes.
    */
    virtual int on_udp_packet(sockaddr_in* from, char* buf, int nb_buf) = 0;
};


/**
* bind udp port, start thread to recv packet and handler it.
*/
class SrsUdpListener : public ITthreadHandle
{
private:
    int _fd;
    st_netfd_t _stfd;
    STThread* pthread;
private:
    char* buf;
    int nb_buf;
private:
    ISrsUdpHandler* handler;
    std::string ip;
    int port;
public:
    SrsUdpListener(ISrsUdpHandler* h, std::string i, int p);
    virtual ~SrsUdpListener();
public:
    virtual int fd();
    virtual st_netfd_t stfd();
public:
    virtual int listen();
// interface ITthreadHandle.
public:
    virtual int cycle();
};


#endif