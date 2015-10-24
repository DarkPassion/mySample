#include <stdio.h>
#include <st_thread.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <st_help.h>
#include <st_udp.h>
#include <st_tcp.h>
#include <vector>


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


class TcpCli : public ITthreadHandle
{

public:
    TcpCli()
    {
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

        char read_buff[256] = {0};
        sprintf(read_buff, "helloworld!");
        int n_write = st_write(_stfd, read_buff, strlen(read_buff), ST_UTIME_NO_TIMEOUT);
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
        const int read_time_out_ms = 3000 * 1000;
        const int write_time_out_ms = 3000 * 1000;

        int n_read = st_read(_stfd, read_buff, 256, read_time_out_ms);
        if (n_read == -1) {
            _pth->stop_loop();
            return -1;
        }

        int n_write = st_write(_stfd, read_buff, strlen(read_buff), write_time_out_ms);
        if (n_write == -1) {
            _pth->stop_loop();
            return -1;
        } 

        printf("cli : n_read [%d] n_write[%d]\n", n_read, n_write);
        //st_usleep(1000 * 100);
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

class TcpConn : public ITthreadHandle
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

    virtual int cycle()
    {

        char read_buff[256] = {0};
        int ccount = 0;
        while(1) {
            memset(read_buff, 0, sizeof(read_buff));
            int n_read = st_read(_stfd, read_buff, 256, ST_UTIME_NO_TIMEOUT);

            int n_write = st_write(_stfd, read_buff, strlen(read_buff), ST_UTIME_NO_TIMEOUT);

            printf("Svr: n_read [%d] n_write [%d] buff [%s]\n", n_read, n_write, read_buff);
            ccount++;
            if (ccount == 10) {
                break;
            }
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
    }
    delete (*iter);
}




int main()
{
    st_init();
    // UdpServ serv;

    // serv.listen();

    // UdpCli cli;

    TcpSvr tcpSvr;
    tcpSvr.listen();

    // TcpCli will delete self
    TcpCli* tcpCli = new TcpCli();

    while (1) {
        st_usleep(1000 * 100);

        //printf("zhangzhifan test\n");
    }
    
    return 0;
}
