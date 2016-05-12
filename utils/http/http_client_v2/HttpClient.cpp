//
// Created by zhangzhifan on 16/5/11.
//

#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netdb.h>
#include <sstream>
#include <string>
#include "HttpClient.h"


HttpClient::HttpClient()
{
    _parser = new HttpParser;
    _fd = -1;
}

HttpClient::~HttpClient()
{
    delete _parser;

    if (_fd != -1)
    {
        close(_fd);
    }

    while (_queue.size()) {
        http_parasm_t* p = _queue.at(0);
        delete p;
        _queue.erase(_queue.begin());
    }

}


void HttpClient::get_response()
{

    _parser->init_with(HTTP_RESPONSE);

    while (1)
    {
        char buff[8192] = {0};
        int nrecv = ::recv(_fd, buff, sizeof(buff), 0);

        if (nrecv <= 0)
        {
            break;
        }

        printf("get response nrecv [%d] \n", nrecv);

        int complete = _parser->parser_content(buff, nrecv);

        if (complete)
        {
            // content complete
            printf("get_response content complete ! \n");
            break;
        }
    }

    // response http header params
    http_params_queue q = _parser->get_http_header_params();
    std::string body = _parser->get_http_body_content();
}

void HttpClient::init_url(int type, const char *url)
{

    _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (_fd < 0)
    {
        _error = HTTP_CLIENT_SOCKET_ERROR;
        return;
    }

    int ret = -1;

    char host[256] = {0};
    char query[256] = {0};
    int port = 0;

    ret = parser_url(url, host, query, port);
    if (ret < 0)
    {
        _error = HTTP_CLIENT_PARSE_URL_ERROR;
        return;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host);

    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        struct hostent *hp = gethostbyname(host);
        if (!hp || !hp->h_addr)
        {
            _error = HTTP_CLIENT_DSN_RESOLVER_ERROR;
            return;
        }
        addr.sin_addr = *(struct in_addr *)hp->h_addr;
    }

    ret = ::connect(_fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0)
    {
        _error = HTTP_CLIENT_CONNECT_ERROR;
        return;
    }

    std::string msg = generate_request(type, host, query, port);
    printf("http client send message [%s] \n", msg.c_str());
    int len = strlen(msg.c_str());

    while (len > 0)
    {
        int nsend = send(_fd, msg.c_str(), len, 0);
        printf("sys send [%d %d] \n", nsend, len);
        if (nsend <= 0)
        {
            // error
            if (errno == EAGAIN)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        len = len - nsend;
    }

}

std::string HttpClient::generate_request(int type, const char *host, const char *query, int port)
{
    std::ostringstream ss;
    const char * sep = "\r\n";
    if (type == HTTP_GET)
    {
        ss << "GET ";
    }
    else
    {
        ss << "POST ";
    }
    ss << query <<" HTTP/1.1" << sep;
    ss << "host: " << host << sep;
    ss << "User-Agent: HttpClient/1.0" << sep;
    ss << "Accept: */*" << sep;

    for (int i = 0; i < _queue.size(); ++i) {
        http_parasm_t* p = _queue.at(i);
        ss << p->key << ": " << p->val << sep;
    }
    ss << sep;

    std::string msg = ss.str();


    return msg;
}


void HttpClient::make_no_pipe(int fd)
{
    const int value = 1;
    if (true)
    {
        setsockopt(_fd, SOL_SOCKET, SO_NOSIGPIPE, &value, sizeof(value));
    }
}

void HttpClient::set_socket_noblocking(int fd)
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



int HttpClient::parser_url(const char* url, char* host, char* query, int & port)
{

    char* p = NULL;

    p = strstr(url, "://");
    if (p == NULL)
    {
        printf("unknow protocal [%s]\n", url);
        return -1;
    }

    int len = int(p - url);
    if (len == 4 && strncasecmp(url, "http", 4) == 0)
    {
        printf("got protocal http \n");
        port = 80;
    }
    else if (len == 5 && strncasecmp(url, "https", 5) == 0)
    {
        // FIXME: https support
        printf("got protocal https\n");
        port = 443;
    }
    else
    {
        printf("got protocal unknow\n");
        return -1;
    }

    // skip "://"
    p = p + 3;

    char *slash = strchr(p, '/');
    char* p2 = strchr(p, ':');

    if (slash == NULL && p2 == NULL)
    {
        printf("error !\n");
        return -1;
    }

    // get host
    if (true)
    {
        int hlen1 , hlen2;
        hlen1 = hlen2 = 0;
        if (slash)
        {
            hlen1 = int(slash - p);
        }

        if (p2)
        {
            hlen2 = int(p2 - p);
        }

        if (hlen2 > 0)
        {
            memcpy(host, p, hlen2);
        }
        else if (hlen2 == 0 && hlen1 > 0)
        {
            memcpy(host, p, hlen1);
        }

        printf("got host [%s]\n", host);
    }

    // get port
    if (true)
    {
        if (p2 && slash)
        {
            int plen = int(slash - p2);
            char pbuf[8] = {0};
            memcpy(pbuf, p2 + 1, plen - 1);
            port = atoi(pbuf);
            printf("got port [%d]\n", port);
        }
        else if (p2)
        {
            char pbuf[8] = {0};
            int plen = (int)strlen(p2);
            memcpy(pbuf, p2 + 1, plen - 1);
            port = atoi(pbuf);
            printf("got port [%d]\n", port);
        }
    }


    // get query
    if (true)
    {

        if (slash)
        {
            int qlen = (int)strlen(slash);
            memcpy(query, slash, qlen);
            printf("got query %s\n", query);
        } else {
            query[0] = '/';
        }

    }
    return 0;
}


void HttpClient::add_params(const char *key, const char *val)
{
    http_parasm_t* param = new http_parasm_t;
    param->key = key;
    param->val = val;
    _queue.push_back(param);

    printf("add params [%s %s] \n", key, val);
}