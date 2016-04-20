#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// libevent
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>

#include <event2/event.h>
#include <errno.h>

// g++ libevent_echo.cpp /usr/local/lib/libevent.a -lrt -I/usr/local/include
// sudo ./a.out

event_base*	base;

const int port = 1020;
const char* ip = "127.0.0.1";
static const char MESSAGE[] = "Hello, World!";


void run_echo_client();

void run_echo_server();



static void echo_client_readcb(struct bufferevent *bev, void *ctx)
{
    char buff[1024] = {0};

    int nread = bufferevent_read(bev, buff, sizeof(buff));

    int nwrite = bufferevent_write(bev, buff, nread);
    printf("echo_client_readcb called [%d %d %s]!\n", nread, nwrite, buff);

}

static void echo_client_eventcb(struct bufferevent *bev, short events, void *ctx)
{
    printf("echo_client_eventcb called [%d %d]\n", events, BEV_EVENT_CONNECTED);

    if (events & BEV_EVENT_EOF)
    {
        printf(" echo_client_eventcb Connection closed.\n");
        bufferevent_free(bev);
    }
    else if (events & BEV_EVENT_ERROR)
    {
        bufferevent_free(bev);
    }

}

void run_echo_client()
{

    if (base == NULL)
    {
        printf("event_base base ! \n");
        return;
    }

    struct sockaddr_storage connect_to_addr;
    int connect_to_addrlen = 0;
    struct bufferevent * b_out = NULL;

    memset(&connect_to_addr, 0, sizeof(connect_to_addr));
    connect_to_addrlen = sizeof(connect_to_addr);

    char buf[256] = {0};
    sprintf(buf, "%s:%d", ip, port);
    if (evutil_parse_sockaddr_port(buf, (struct sockaddr*)&connect_to_addr, &connect_to_addrlen)<0)
    {
        printf("evutil_parse_sockaddr_port error !\n");
        return;
    }

    b_out = bufferevent_socket_new(base, -1,
                                   BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);

    if (bufferevent_socket_connect(b_out,
                                   (struct sockaddr*)&connect_to_addr, connect_to_addrlen)<0)
    {

        printf( "bufferevent_socket_connect error !\n" );
        bufferevent_free(b_out);
        return;
    }

    bufferevent_setcb(b_out, echo_client_readcb, NULL, echo_client_eventcb, base);
    bufferevent_enable(b_out, EV_READ|EV_WRITE);


}




static void conn_readcb(struct bufferevent *bev, void *user_data)
{

    char buff[1024] = {0};

    int nread = bufferevent_read(bev, buff, sizeof(buff));

    int nwrite = bufferevent_write(bev, buff, nread);
    printf("conn_readcb called [%d %d %s]!\n", nread, nwrite, buff);


}



static void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
    if (events & BEV_EVENT_EOF)
    {
        printf("Connection closed.\n");
    }
    else if (events & BEV_EVENT_ERROR)
    {
        printf("Got an error on the connection: %s\n",
               strerror(errno));/*XXX win32*/
    }
    /* None of the other events can happen here, since we haven't enabled
     * timeouts */
    bufferevent_free(bev);
}


static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
                        struct sockaddr *sa, int socklen, void *user_data)
{
    struct event_base *base = (struct event_base*)user_data;
    struct bufferevent *bev;

    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
    {
        fprintf(stderr, "Error constructing bufferevent!");
        event_base_loopbreak(base);
        return;
    }
    bufferevent_setcb(bev, conn_readcb, NULL, conn_eventcb, NULL);
    bufferevent_enable(bev, EV_WRITE|EV_READ);
    bufferevent_write(bev, MESSAGE, strlen(MESSAGE));

}

void run_echo_sever()
{

    struct evconnlistener *listener;
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
                                       LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
                                       (struct sockaddr*)&sin,
                                       sizeof(sin));

    if (!listener)
    {
        printf("could not create listener ! \n");
        return;
    }




}



int main()
{

    base = event_base_new();




    run_echo_sever();

    run_echo_client();

    event_base_dispatch(base);

    event_base_free(base);

    return 0;
}
