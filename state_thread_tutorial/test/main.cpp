#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <st_thread.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <st_help.h>
#include <st_udp.h>
#include <st_tcp.h>
#include <st_msg.h>
#include <vector>
#include <st_queue.h>

class UdpCli : public ITthreadHandle
{
public:
    UdpCli()
    {
        _pth = new STThread("UdpCli", this,  100, false);
        _fd = socket(AF_INET, SOCK_DGRAM, 0);
        _stfd = st_netfd_open_socket(_fd);

        _dst.sin_family = AF_INET;
        _dst.sin_port = htons(8080);
        _dst.sin_addr.s_addr = inet_addr("127.0.0.1");
        _pth->start();
    }

    ~UdpCli()
    {
        srs_close_stfd(_stfd);

        _pth->stop();
        srs_freep(_pth);
        close(_fd);
    }


    virtual int cycle()
    {
        char buff[256] = {0};
        sprintf(buff, "helloworld !\r\n");
        st_sendto(_stfd, buff, strlen(buff), (const sockaddr*)&_dst, sizeof(sockaddr_in), ST_UTIME_NO_TIMEOUT);

        st_usleep(1000 * 100);
        return 0;
    }

private:
    STThread* _pth;
    int _fd;
    st_netfd_t _stfd;
    sockaddr_in _dst;
};



class UdpServ: public ISrsUdpHandler
{
public:
    UdpServ()
    {

        listener = new SrsUdpListener(this, "127.0.0.1", 8080);
    }

    ~UdpServ()
    {
        delete listener;
    }

    virtual int on_udp_packet(sockaddr_in* from, char* buf, int nb_buf)
    {

        printf("on udp packet len %d recv %s \n", nb_buf, buf);
        return 0;
    }

    void listen()
    {
        listener->listen();
    }

private:
    SrsUdpListener* listener;
};

/**
 st_msg.h
  class Message
*/
class TcpCli : public ITthreadHandle
{

public:
    TcpCli()
    {
        _count = 0;
        _pth = new STThread("TcpCli", this,  100, false);

        _fd = socket(AF_INET, SOCK_STREAM, 0);
        _stfd = st_netfd_open_socket(_fd);

        sockaddr_in _dst;
        _dst.sin_family = AF_INET;
        _dst.sin_port = htons(8090);
        _dst.sin_addr.s_addr = inet_addr("127.0.0.1");

        const int connnect_time_out_ms = 5000 * 1000;
        int conn_ret = st_connect(_stfd, (const sockaddr*)&_dst, sizeof(sockaddr_in), connnect_time_out_ms);
        if (conn_ret == -1) {
            fprintf(stderr, "st_connect timeout [%d]\n", connnect_time_out_ms);
        }

        _pth->start();

       
        char write_buff[256] = {0};
        const char* write_msg = "helloworld!";
        EncodeMsg::parse_message(1122, strlen(write_msg), write_msg, write_buff);

        //DecodeMsg de;
        //de.parse_data(strlen(write_msg) + 8, write_buff);


        int n_write = st_write(_stfd, write_buff, strlen(write_msg) + 4 + 4, ST_UTIME_NO_TIMEOUT);
        printf("Cli: n_write [%d] msg[%s] \n", n_write, write_buff + 8);
        n_write = st_write(_stfd, write_buff, strlen(write_msg) + 4 + 4, ST_UTIME_NO_TIMEOUT);
        printf("Cli: n_write [%d] msg[%s]\n", n_write, write_buff + 8);
        
    }

    ~TcpCli()
    {
        _pth->stop();
        srs_close_stfd(_stfd);
        srs_freep(_pth);
        close(_fd);
    }

    virtual int cycle()
    {
        char read_buff[256] = {0};
        const int write_time_out_ms = 3000 * 1000;

        //gets(read_buff);
        const char* write_msg = "helloworld1";
        EncodeMsg::parse_message(4346, strlen(write_msg), write_msg, read_buff);
        
        
        int n_write = st_write(_stfd, read_buff, strlen(write_msg) + 8, write_time_out_ms);

        DecodeMsg de;
        de.parse_data(strlen(write_msg) + 8, read_buff);

        printf("Cli: n_write [%d] msg [%s]\n", n_write, read_buff + 8);
        if (n_write == -1) {
            _pth->stop_loop();
            return -1;
        } 

        st_usleep(1000 * 5000);
        return 0;
    }

    virtual void on_thread_stop()
    {
        printf("TCPCli on_thread_stop \n");
        delete this;
    }


private:
    STThread* _pth;
    int _fd;
    st_netfd_t _stfd;
    int _count;
};


class TcpConn;
class TcpSvr : public ISrsTcpHandler
{
public:
    TcpSvr();
    ~TcpSvr();
    virtual int on_tcp_client(st_netfd_t stfd);

    void listen();

    void remove(TcpConn* conn); 

private:
    SrsTcpListener* _listener;

    typedef std::vector<TcpConn*> NetFdQueue;
    NetFdQueue  _queue;
};


class TcpConn : public ITthreadHandle, public ICodecMsg
{
public:
    TcpConn(st_netfd_t stfd, TcpSvr* svr)
    {
        _svr = svr;
        _stfd = stfd;
        _pth = new STThread("TcpConn", this,  100, false);

        _pth->start();
    }

    ~TcpConn()
    {
        srs_close_stfd(_stfd);
         _pth->stop();
        srs_freep(_pth);
    }

    virtual void on_message(Message* msg)
    {

        printf("Svr: on_message type [%d] len [%d] msg[%s]\n", msg->type, msg->len, msg->data);
    }

    virtual int cycle()
    {

        char read_buff[256] = {0};
        DecodeMsg de;
        while (1) {
            memset(read_buff, 0, sizeof(read_buff));
            int n_read = st_read(_stfd, read_buff, sizeof(read_buff), ST_UTIME_NO_TIMEOUT);
            // printf("Svr: n_read[%d] \n", n_read);

            if (n_read == -1) {
                break;
            }

            de.parse_data(n_read, read_buff);

        }

        _pth->stop_loop();
        return 0;
    }

    
    virtual void on_thread_stop()
    {
        _svr->remove(this);
    }

private:
    STThread* _pth;
    st_netfd_t _stfd;
    TcpSvr* _svr;

};


TcpSvr::TcpSvr()
{
    _listener = new SrsTcpListener(this, "127.0.0.1", 8090);
}

TcpSvr::~TcpSvr()
{
    delete _listener;
}

int TcpSvr::on_tcp_client(st_netfd_t stfd)
{
    TcpConn* conn = new TcpConn(stfd, this);
    _queue.push_back(conn);
    return 0;
}

void TcpSvr::listen()
{
    _listener->listen();
}

void TcpSvr::remove(TcpConn* conn)
{
    NetFdQueue::iterator iter = std::find(_queue.begin(), _queue.end(), conn);
    if (iter != _queue.end()) {
        _queue.erase(iter);
        delete (*iter);
    }
}




class TestQueuePush : public ITthreadHandle
{
public:

    TestQueuePush(BolckQueue<int> * q)
    {
        _pth = new STThread("TestQueuePush", this, 0, false);
        _pth->start();
        _queue = q;
    }

    ~TestQueuePush()
    {
        _pth->stop();
    }

    virtual int cycle()
    {
        int i = 0;
        while(_pth->can_loop()) {

            printf("QPush --- %d\n", i);
            _queue->push_back(i++); 
            st_usleep(1000* 100);
        }
        return 0;
    }

private:
    BolckQueue<int>* _queue;
    STThread* _pth;
};


class TestQueuePop : public ITthreadHandle
{

public:
    TestQueuePop(BolckQueue<int>* q)
    {
        _queue = q;
        _pth = new STThread("TestQueuePop", this, 0, false);
        _pth->start();
    }

    ~TestQueuePop()
    {
        _pth->stop();
    }


    virtual int cycle()
    {
        int i = 0;

        while(_pth->can_loop()) {

            //int i =  _queue->pop_front();
			// this with timeout params
			int i = _queue->pop_front(10);
			printf("Qpop --- %d \n", i);
            //st_usleep(1000 * 10);
        }
        return 0;
    }
private:
    STThread*   _pth;
    BolckQueue<int>* _queue;
};


int main()
{

    st_init();

    BolckQueue<int> q;

    //TestQueuePush tq1(&q);
    //TestQueuePop tq2(&q);
    // UdpServ serv;

    // serv.listen();

    // UdpCli cli;

    TcpSvr tcpSvr;
    tcpSvr.listen();

    // TcpCli will delete self
    TcpCli* tcpCli = new TcpCli();

    while (1) {
        st_usleep(1000 * 100);
    }
    
    return 0;
}
