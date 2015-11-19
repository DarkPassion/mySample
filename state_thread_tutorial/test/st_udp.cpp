#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <st_udp.h>
#include <srs_core.h>
#include <st_help.h>
#include <string>

// set the max packet size.
#define SRS_UDP_MAX_PACKET_SIZE 65535

// sleep in ms for udp recv packet.
#define SRS_UDP_PACKET_RECV_CYCLE_INTERVAL_MS 0

// nginx also set to 512
#define SERVER_LISTEN_BACKLOG 512

ISrsUdpHandler::ISrsUdpHandler()
{
}

ISrsUdpHandler::~ISrsUdpHandler()
{
}

int ISrsUdpHandler::on_stfd_change(st_netfd_t /*fd*/)
{
    return 0;
}



SrsUdpListener::SrsUdpListener(ISrsUdpHandler* h, std::string i, int p)
{
    handler = h;
    ip = i;
    port = p;

    _fd = -1;
    _stfd = NULL;

    nb_buf = SRS_UDP_MAX_PACKET_SIZE;
    buf = new char[nb_buf];

    pthread = new STThread("udp", this,  100, true);
}

SrsUdpListener::~SrsUdpListener()
{
    // close the stfd to trigger thread to interrupted.
    srs_close_stfd(_stfd);

    pthread->stop();
    srs_freep(pthread);
    
    // st does not close it sometimes, 
    // close it manually.
    close(_fd);

    srs_freep(buf);
}

int SrsUdpListener::fd()
{
    return _fd;
}

st_netfd_t SrsUdpListener::stfd()
{
    return _stfd;
}

int SrsUdpListener::listen()
{
    int ret = 0;
    
    if ((_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        ret = ERROR_SOCKET_CREATE;
        fprintf(stderr, "create linux socket error. ip=%s, port=%d, ret=%d\n", ip.c_str(), port, ret);
        return ret;
    }
    fprintf(stderr, "create linux socket success. ip=%s, port=%d, fd=%d\n", ip.c_str(), port, _fd);
    
    int reuse_socket = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_socket, sizeof(int)) == -1) {
        ret = ERROR_SOCKET_SETREUSE;
        fprintf(stderr, "setsockopt reuse-addr error. ip=%s, port=%d, ret=%d\n", ip.c_str(), port, ret);
        return ret;
    }
    fprintf(stdout, "setsockopt reuse-addr success. ip=%s, port=%d, fd=%d\n", ip.c_str(), port, _fd);
    
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (bind(_fd, (const sockaddr*)&addr, sizeof(sockaddr_in)) == -1) {
        ret = ERROR_SOCKET_BIND;
        fprintf(stderr, "bind socket error. ep=%s:%d, ret=%d\n", ip.c_str(), port, ret);
        return ret;
    }
    fprintf(stdout, "bind socket success. ep=%s:%d, fd=%d\n", ip.c_str(), port, _fd);
    
    if ((_stfd = st_netfd_open_socket(_fd)) == NULL){
        ret = ERROR_ST_OPEN_SOCKET;
        fprintf(stderr, "st_netfd_open_socket open socket failed. ep=%s:%d, ret=%d\n", ip.c_str(), port, ret);
        return ret;
    }
    fprintf(stdout, "st open socket success. ep=%s:%d, fd=%d\n", ip.c_str(), port, _fd);
    
    if ((ret = pthread->start()) != ERROR_SUCCESS) {
        fprintf(stderr, "st_thread_create listen thread error. ep=%s:%d, ret=%d\n", ip.c_str(), port, ret);
        return ret;
    }
    fprintf(stdout, "create st listen thread success, ep=%s:%d\n", ip.c_str(), port);

    return ret;
}

int SrsUdpListener::cycle()
{
    int ret = ERROR_SUCCESS;

    // TODO: FIXME: support ipv6, @see man 7 ipv6
    sockaddr_in from;
    int nb_from = sizeof(sockaddr_in);
    int nread = 0;

    if ((nread = st_recvfrom(_stfd, buf, nb_buf, (sockaddr*)&from, &nb_from, ST_UTIME_NO_TIMEOUT)) <= 0) {
        fprintf(stderr, "ignore recv udp packet failed, nread=%d\n", nread);
        return ret;
    }
    
    if ((ret = handler->on_udp_packet(&from, buf, nread)) != ERROR_SUCCESS) {
        fprintf(stderr, "handle udp packet failed. ret=%d\n", ret);
        return ret;
    }

    if (SRS_UDP_PACKET_RECV_CYCLE_INTERVAL_MS > 0) {
        st_usleep(SRS_UDP_PACKET_RECV_CYCLE_INTERVAL_MS * 1000);
    }

    return ret;
}

