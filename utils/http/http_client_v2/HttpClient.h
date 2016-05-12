//
// Created by zhangzhifan on 16/5/11.
//

#ifndef CLIONTEST_HTTPCLIENT_H
#define CLIONTEST_HTTPCLIENT_H


class HttpParser;
class HttpClient
{

public:

    enum
    {
        HTTP_GET,
        HTTP_POST
    };

    enum
    {
        HTTP_CLIENT_SOCKET_ERROR = -100,
        HTTP_CLIENT_CONNECT_ERROR,
        HTTP_CLIENT_PARSE_URL_ERROR,
        HTTP_CLIENT_DSN_RESOLVER_ERROR
    };

    HttpClient();

    ~HttpClient();


    void init_url(int type, const char * url);


    void get_response();

private:

    HttpClient(const HttpClient& c);
    HttpClient& operator=(const HttpClient& c);

    void make_no_pipe(int fd);

    void set_socket_noblocking(int fd);

    int parser_url(const char* url, char* host, char* query, int & port);

    HttpParser*         _parser;
    int                 _fd;
    int                 _error;
};


#endif //CLIONTEST_HTTPCLIENT_H
