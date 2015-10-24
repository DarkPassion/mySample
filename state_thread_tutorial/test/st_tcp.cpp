#include <st_tcp.h>
#include <srs_core.h>
#include <st_help.h>

// nginx also set to 512
#define SERVER_LISTEN_BACKLOG 512

ISrsTcpHandler::ISrsTcpHandler()
{
}

ISrsTcpHandler::~ISrsTcpHandler()
{
}


SrsTcpListener::SrsTcpListener(ISrsTcpHandler* h, std::string i, int p)
{

	handler = h;
    ip = i;
    port = p;

    _fd = -1;
    _stfd = NULL;

	pthread = new STThread("tcpListener", this,  100, true);
}


SrsTcpListener::~SrsTcpListener()
{
	 // close the stfd to trigger thread to interrupted.
    srs_close_stfd(_stfd);

    pthread->stop();
    srs_freep(pthread);
    
    // st does not close it sometimes, 
    // close it manually.
    close(_fd);
}

int SrsTcpListener::fd()
{
	return _fd;
}


int SrsTcpListener::listen()
{
    int ret = ERROR_SUCCESS;
    
    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        ret = ERROR_SOCKET_CREATE;
        fprintf(stderr, "create linux socket error. port=%d, ret=%d\n", port, ret);
        return ret;
    }
    fprintf(stdout, "create linux socket success. port=%d, fd=%d\n", port, _fd);
    
    int reuse_socket = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_socket, sizeof(int)) == -1) {
        ret = ERROR_SOCKET_SETREUSE;
        fprintf(stderr, "setsockopt reuse-addr error. port=%d, ret=%d\n", port, ret);
        return ret;
    }
    fprintf(stdout, "setsockopt reuse-addr success. port=%d, fd=%d\n", port, _fd);
    
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
    
    if (::listen(_fd, SERVER_LISTEN_BACKLOG) == -1) {
        ret = ERROR_SOCKET_LISTEN;
        fprintf(stderr, "listen socket error. ep=%s:%d, ret=%d\n", ip.c_str(), port, ret);
        return ret;
    }
    fprintf(stdout, "listen socket success. ep=%s:%d, fd=%d\n", ip.c_str(), port, _fd);
    
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




int SrsTcpListener::cycle()
{
    int ret = ERROR_SUCCESS;
    
    st_netfd_t client_stfd = st_accept(_stfd, NULL, NULL, ST_UTIME_NO_TIMEOUT);
    
    if(client_stfd == NULL){
        // ignore error.
        fprintf(stderr, "ignore accept thread stoppped for accept client error\n");
        return ret;
    }
    fprintf(stdout, "get a client. fd=%d\n", st_netfd_fileno(client_stfd));
    
    if ((ret = handler->on_tcp_client(client_stfd)) != ERROR_SUCCESS) {
        fprintf(stderr, "accept client error. ret=%d\n", ret);
        return ret;
    }
    
    return ret;
}