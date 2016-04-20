
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// libevent
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/http.h>



// g++ libevent_test.cpp /usr/local/lib/libevent.a -lrt -I/usr/local/include


void test_libevent_timer();


void test_libevent_connect();


void test_libevent_http_client();



struct timeval _lasttime = {0};
static void timeout_cb(evutil_socket_t fd, short event, void *arg)
{

    struct timeval newtime = {0};
    struct timeval diff = {0};
    evutil_gettimeofday(&newtime, NULL);
    evutil_timersub(&newtime, &_lasttime, &diff);
    int elapsed  = diff.tv_sec * 1000 + diff.tv_usec/1000;

    printf("timeoutcb called [%d %d %d]\n",fd, event, elapsed);

    struct event *timeout = (struct event*)arg;
    struct timeval tv;
    evutil_timerclear(&tv);
    tv.tv_sec = 2;
    event_add(timeout, &tv);


}

void test_libevent_timer()
{
    struct event timeout;
    struct timeval tv;
    struct event_base* base = event_base_new();

    event_assign(&timeout, base, -1, 0, timeout_cb, (void*)&timeout);

    evutil_gettimeofday(&_lasttime, NULL);
    evutil_timerclear(&tv);
    tv.tv_sec = 2;
    event_add(&timeout, &tv);

    event_base_dispatch(base);
}


static void eventcb(struct bufferevent *bev, short what, void *ctx)
{
    printf("eventcb called [%d %d]\n", what, BEV_EVENT_CONNECTED);

    struct timeval tv = {0};
    tv.tv_sec = 2;
    bufferevent_free(bev);

    evutil_closesocket(bufferevent_getfd(bev));
    struct event_base* base = (struct event_base*)ctx;
    event_base_loopexit(base, &tv);
    printf("event base will lopp exit 2s!");
}

static void readcb(struct bufferevent *bev, void *ctx)
{
    printf("readcb called !\n");
}

void test_libevent_connect()
{
    struct sockaddr_storage connect_to_addr;
    int connect_to_addrlen = 0;
    struct bufferevent * b_out = NULL;
    struct event_base* base  = NULL;

    base = event_base_new();
    memset(&connect_to_addr, 0, sizeof(connect_to_addr));
    connect_to_addrlen = sizeof(connect_to_addr);

    if (evutil_parse_sockaddr_port("123.125.114.144:80", (struct sockaddr*)&connect_to_addr, &connect_to_addrlen)<0)
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

    bufferevent_setcb(b_out, readcb, NULL, eventcb, base);
    bufferevent_enable(b_out, EV_READ|EV_WRITE);

    event_base_dispatch(base);
}


static void http_request_done(struct evhttp_request *req, void *ctx)
{
    unsigned char buf[1024];
    int s;
    int i;
    int l;

    memset(buf,0,sizeof(buf));

    if (req == NULL)
    {
        printf("Connect refused!\n");
    }
    else if (evhttp_request_get_response_code(req) == 0)
    {
        printf("Time out!\n");
    }
    else if (evhttp_request_get_response_code(req) != 200)
    {
        printf("error: %u %s\n", evhttp_request_get_response_code(req), evhttp_request_get_response_code_line(req));
    }
    else
    {
        printf("success: %u %s\n", evhttp_request_get_response_code(req), evhttp_request_get_response_code_line(req));
        s = evbuffer_remove(evhttp_request_get_input_buffer(req), &buf, sizeof(buf) - 1);
        buf[s] = '\0';
        printf("%s", buf);

    }

    event_base_loopbreak((struct event_base *)ctx);

}


void test_libevent_http_client()
{
    struct event_base *base;
    struct evhttp_connection *conn;
    struct evhttp_request *req;


    base = event_base_new();
    conn = evhttp_connection_base_new(base, NULL, "127.0.0.1", 80);
    req = evhttp_request_new(http_request_done, base);

    struct evkeyvalq *output_headers = evhttp_request_get_output_headers(req);
    evhttp_add_header(output_headers, "Connection", "close");
    evhttp_add_header(output_headers, "Host", "localhost");
    evhttp_make_request(conn, req, EVHTTP_REQ_GET, "/");
    evhttp_connection_set_timeout(conn, 5);

    event_base_dispatch(base);
    evhttp_connection_free(conn);
    event_base_free(base);
}


int main()
{

    // test_libevent_timer();

    // test_libevent_connect();

    // test_libevent_http_client();

    return 0;
}


