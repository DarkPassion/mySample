#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <fcntl.h>
#include <string.h>
#include <map>
#include <vector>

// g++ http_server_v2.cpp -o http_server_v2.out

void set_socket_noblocking(int fd);

void set_socket_reuseable(int fd);


void on_accept(int fd, int event, void* ptr);

void on_server_read(int fd, int event, void* u);

void on_client_read(int fd, int event, void* u);

void on_client_write(int fd, int event, void* u);


void socket_no_pipe(int fd);

int mylistener(int port);

void http_url_parse(const char* url);


class Reactor;
int myclient(int port, Reactor* acotor);


enum
{
    REACTOR_EVENT_READ,
    REACTOR_EVENT_WRITE,

};

typedef void (*ReactorCallback)(int fd, int event, void* ptr);


class Reactor
{

public:
    Reactor()
    {
        _stop = 0;
        _status = 0;
    }

    ~Reactor()
    {

    }

    void run_loop()
    {
        _status = 1;
        while(!_stop)
        {
            struct timeval tv;
            tv.tv_sec = 1;
            tv.tv_usec = 0;

            fd_set rfds;
            fd_set wfds;
            FD_ZERO(&rfds);
            FD_ZERO(&wfds);
            int max_fd = 0;

            if (true)
            {
                int i = 0;
                int s = (int)_fds.size();
                while (i < s)
                {
                    RectorParams* p = _fds.at(i);

                    if (p->_read_callback && p->_fd)
                    {
                        FD_SET(p->_fd, &rfds);
                    }

                    if (p->_write_callback && p->_fd)
                    {
                        FD_SET(p->_fd, &wfds);
                    }

                    if (p->_fd > max_fd)
                    {
                        max_fd = p->_fd;
                    }
                    i++;
                }
            }

            int nselect = select(max_fd + 1, &rfds, &wfds, NULL, &tv);

            if (nselect == 0)
            {
                printf("select [%d %zu]\n", max_fd, _fds.size());
                // timeout
                continue;
            }
            else if (nselect == -1)
            {
                // select error !
                break;
            }
            else if (nselect > 0)
            {

                if (true)
                {
                    int i = 0;

                    printf(" event fds [%zu]\n", _fds.size());

                    while (i< _fds.size())
                    {

                        RectorParams* p = _fds.at(i);

                        if (p && p->_fd && FD_ISSET(p->_fd, &rfds) && p->_read_callback)
                        {
                            p->_read_callback(p->_fd, REACTOR_EVENT_READ, p->_userdata);
                        }

                        if (p && p->_fd && FD_ISSET(p->_fd, &wfds) && p->_write_callback)
                        {
                            p->_write_callback(p->_fd, REACTOR_EVENT_WRITE, p->_userdata);
                            _fds.erase(_fds.begin() + i);
                            delete p;
                            p = NULL;
                            continue;
                        }

                        i++;
                    }
                }

            }

        }
        _status = 2;
    }


    void add_event(int fd, ReactorCallback read, ReactorCallback write, void* userdata)
    {
        RectorParams* p = new RectorParams;
        p->_fd = fd;
        p->_read_callback = read;
        p->_write_callback = write;
        p->_userdata = userdata;
        _fds.push_back(p);

        if (fd < 0)
        {
            printf("add event [%d] \n", fd);
        }

    }


    void exit()
    {

        _stop = 1;
    }

private:


    struct RectorParams
    {
        int             _fd;
        ReactorCallback _read_callback;
        ReactorCallback _write_callback;
        void*           _userdata;

    };

    Reactor (const Reactor& r);

    void operator=(const Reactor& r);

    typedef std::vector<RectorParams*> VectFds;
    VectFds _fds;
    int     _stop;
    int     _status;

};


void set_socket_noblocking(int fd)
{
    if (true)
    {
        int flags;
        if ((flags = fcntl(fd, F_GETFL, NULL)) < 0)
        {
            printf("fcntl F_GETFL fail !\n");
            return;
        }
        if (!(flags & O_NONBLOCK))
        {
            if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
            {
                printf("fcntl F_SETFL fail\n");
                return;
            }
        }
    }

}



void set_socket_reuseable(int fd)
{
    int one = 1;
    /* REUSEADDR on Unix means, "don't hang on to this address after the
     * listener is closed."  On Windows, though, it means "don't keep other
     * processes from binding to this address while we're using it. */
    socklen_t len = sizeof(one);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*) &one, len);
}


int mylistener(int port, Reactor* actor)
{

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("socket error!\n");
        return -1;
    }

    set_socket_reuseable(fd);
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0)
    {
        printf("bind error!\n");
        return -1;
    }
    printf("bind SUCC!\n");

    ret = listen(fd, 10);

    if (ret < 0)
    {
        printf("listen error!\n");
        return -1;
    }

    set_socket_noblocking(fd);
    actor->add_event(fd, on_accept, NULL, actor);

    return 0;
}

void on_accept(int fd, int event, void* u)
{

    Reactor* actor =  (Reactor*)u;
    printf("begin accept\n");
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    memset(&client_addr, 0, sizeof(client_addr));
    int clientfd = accept(fd, (struct sockaddr*)&client_addr, &len);
    if (clientfd > 0)
    {
        printf("on_accept [%d %s]\n", clientfd, inet_ntoa(client_addr.sin_addr));
        actor->add_event(clientfd, on_server_read, NULL, actor);
    }
    else
    {
        printf("accept error !\n");
        return;
    }

}


void on_server_read(int fd, int event, void* u)
{
    printf("on server read !\n");
    //Reactor* actor = (Reactor*)u;

    char outbuff[8192] = {0};
    size_t nrecv = recv(fd, outbuff, sizeof(outbuff), 0);
    printf("on_server_read [%d %zu %s] \n", fd, nrecv, outbuff);

    size_t nsend = send(fd, outbuff, strlen(outbuff), 0);
    printf("on_server_read [%zu %s] \n", nsend, outbuff);

}

int myclient(int port, Reactor* actor)
{

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0)
    {
        printf("socket ERR!\n");
        return -1;
    }

    set_socket_noblocking(fd);

    connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    actor->add_event(fd, NULL, on_client_write, actor);


    return 0;
}


void on_client_write(int fd, int event, void* u)
{
    printf("on client write !\n");

    Reactor* actor = (Reactor*)u;

    const char * p = "on client write !";
    size_t nsend = send(fd, p, strlen(p), 0);
    printf("nsend =[%zu %s] \n", nsend, p);
    actor->add_event(fd, on_client_read, NULL, actor);

}

void on_client_read(int fd, int event, void* u)
{
    //Reactor* actor = (Reactor*)u;

    char outbuff[8192] = {0};
    size_t nrecv = recv(fd, outbuff, sizeof(outbuff), 0);
    size_t nsend = send(fd, outbuff, strlen(outbuff), 0);
    printf("on client read [%zu %zu %s]\n",nrecv, nsend, outbuff);

    //actor->add_event(fd, on_client_read, NULL, actor);
}


void socket_no_pipe(int fd)
{
    const int value = 1;
    setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &value, sizeof(value));
}

void http_url_parse(const char* url)
{

    char url_buff[2048] = {0};
    snprintf(url_buff, sizeof(url_buff) - 1, "%s", url);


    // onley handle http protocal
    if (strncasecmp(url_buff, "http", 4)) {

        printf("url [%s] not support!\n", url_buff);
        return ;
    }

    char* p1 = strchr(url_buff + 4, ':');
    if (!p1 || strncmp(p1, "://", 3)) {
        printf("bad request! \n");
        return;
    }

    char host[256] = {0};
    char* p2 = p1 + 3;
    char* p3 = strchr(p2, '/');
    int hlen = (int)(p3 - p2);
    strncpy(host, p2, hlen);

    printf("host :[%s]\n", host);

    p1 = strrchr(host, ':');
    if (p1) {
        *p1++ = '\0';
        int port = atoi(p1);
        printf("port [%d]\n", port);
    }

}


int main()
{

    // Reactor _actor;
    // mylistener(80, &_actor);
    // myclient(80, &_actor);

    // _actor.run_loop();


    const char* u1 = "http://cc.cc.ee:190/ww";
    const char* u2 = "https://www.baidu.com:22/d";
    const char* u3 = "www.baidu.com:22";
    http_url_parse(u1);
    http_url_parse(u2);
    http_url_parse(u3);

    return 0;
}