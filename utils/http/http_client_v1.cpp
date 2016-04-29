#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <string.h>
#include <map>
#include <vector>

// g++ http_client_v1.cpp -o http_client_v1.out
// https://github.com/emilw/HttpClient.git

int http_client_sendmessage(int fd, const char* url);

int http_clent_recvmessage(int fd);

int http_client_dns_resolve(const char * url);

void set_socket_noblocking(int fd);

void http_client_parse_header(char* indata, int inlen, int& content_len, int& header_len);

int get_http_content_len(char* http_response_header);

void parse_http_param(char* indata, int inlen);

struct Params
{
    std::string key;
    std::string val;
};

typedef std::vector<Params*> HttpParam;
// map : when insert pair auto sort
// typedef std::map<std::string, std::string> HttpParam;

//
//  struct sockaddr {
// 　　   unsigned short sa_family; /* address family, AF_xxx */  // AF_INET,AF_INET6,AF_UNSPEC
//　　    char sa_data[14]; /* 14 bytes of protocol address */
//　};

//    struct sockaddr_in {
//    　　short int sin_family; /* Address family */      // AF_INET,AF_INET6,AF_UNSPEC
//    　　unsigned short int sin_port; /* Port number */  // sin_port network byte order
//    　　struct in_addr sin_addr; /* Internet address */
//    　　unsigned char sin_zero[8]; /* Same size as struct sockaddr */
//    　};



//    const char* ip = "127.0.0.1";
//    struct socketaddr_in addr;
//    memset(&addr, 0, sizeof(addr));
//    addr.sin_family = AF_INET;
//    addr.sin_port = htons(80);
//    addr.sin_addr.sin_addr = inet_addr(ip);
//    connect(fd, (struct sockaddr*)&addr, sizeof(addr));


//    struct socketaddr_in client_addr;
//    memset(&client_addr, 0, sizeof(client_addr));
//    accept(fd, (struct sockaddr*)&client_addr, sizeof(client_addr));
//
//    struct sockaddr* src_addr = (struct sockaddr*)&client_addr;
//    char* ipv4 = inet_ntoa(src_addr->sin_addr);
//    printf("ip[%s]\n", ipv4);



int http_client_dns_resolve(const char * url)
{
    struct addrinfo hints, *res, *res0;
    int error;
    int s;
    const char *cause = NULL;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    error = getaddrinfo(url, "http", &hints, &res0);
    
    if (error) {
        printf(" error == %s", gai_strerror(error));
        return -1;
    }
    
    s = -1;
    for (res = res0; res; res = res->ai_next) {
        
        printf("socket == [%d %d]\n", res->ai_socktype, res->ai_protocol);
        s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (s < 0) {
            cause = "socket";
            continue;
        }
        
        // int socket
        // struct sockaddr* address
        // socklen_t address_len
        // connect(int socket, const struct sockaddr *address, socklen_t address_len);

        if (connect(s, res->ai_addr, res->ai_addrlen) < 0) {
            printf("connect error !\n");
            s = -1;
            continue;
        }
        struct sockaddr_in* sockin = (struct sockaddr_in*)(res->ai_addr);
        const char * ip = inet_ntoa(sockin->sin_addr);
        printf("connect succ! [%s]\n", ip);
        break;  /* okay we got one */
    }
    
    freeaddrinfo(res0);

    return s;
}

int http_client_sendmessage(int fd, const char* url)
{
    if (fd < 1 || url == NULL) {
        printf("http client sendmessage params error !\n");
        return -1;
    }

    const char* host = NULL;
    if (true)
    {
        const char* c = strstr(url, "http://");
        if (c != NULL)
        {
            host = c;
        } else {
            host = url;
        }
    }

    std::ostringstream ss;
    ss << "GET / HTTP/1.1\n";
    ss << "host:" << host << "\n";
    ss << "User-Agent: curl/7.43.0\n";
    ss << "Accept: */*\n\n";
    
    std::string msg = ss.str();
    
    // set_socket_noblocking(fd);
    int len = strlen(msg.c_str());
    int nsend = send(fd, msg.c_str(), len, 0);
    
    // printf("send message [%d %s]\n", len, msg.c_str());
    int recvlen = 0;
    int content_len = 0;
    int header_len = 0;
    while (1) {

        fd_set fds;
        int n;
    
        // 3 timeout
        struct timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0;

        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        n = select(fd + 1, &fds, NULL, NULL, &tv);
        if (n ==0)
        {
            printf("timeout === \n");
            break;
        } else if (n == -1)
        {
            printf("select error !\n");
            break;
        }
        
        char outbuff[8192] = {0};
        int nrecv = recv(fd, outbuff, sizeof(outbuff), 0);
        
        if (recvlen == 0)
        {
            http_client_parse_header(outbuff, nrecv, content_len, header_len);
            printf("http_client_parse_header [%d %d]\n", content_len, header_len);
            nrecv -= header_len;
            //printf("%s\n", outbuff);
        }
        recvlen += nrecv;
        printf("nrecv [%d]\n", nrecv);
        if (nrecv <= 0) {
            break;
        }
        
        if (content_len <= recvlen)
        {
            printf("recvlen eq content len\n");
            break;
        }
        //printf("%s", outbuff);
    }
    
    printf("all recv len [%d]\n", recvlen);
    close(fd);
    

    return 0;
}

void set_socket_noblocking(int fd)
{
    if (true)
    {
        int flags;
        if ((flags = fcntl(fd, F_GETFL, NULL)) < 0) {
            printf("fcntl F_GETFL fail !\n");
            return;
        }
        if (!(flags & O_NONBLOCK)) {
            if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
                printf("fcntl F_SETFL fail\n");
                return;
            }
        }
    }

}



void parse_http_param(char* indata, int inlen)
{
    int i = 0;
    const char* sep = "\r\n";
    HttpParam _queue;
    while (i < inlen)
    {
       if (indata[i + 0] == '\r' && indata[i + 1] == '\n')
       {
            indata[i + 0] = 0x0;
            indata[i + 1] = 0x0;
       }
       i++;
    }

    i = 0;
    while (i < inlen)
    {
        int line_len = strlen(indata + i);

        char* line = indata + i;

        for (int j = 0; j < line_len; j++)
        {
            if (line[j] == ':')
            {
                line[j] = 0x0;
                Params *p = new Params;
                std::string key(line);
                std::string val(line + j + 1);
                p->key = key;
                p->val = val;
                _queue.push_back(p);
                break;
            }
        }
        i += line_len + 2;
    }


    HttpParam::iterator iter = _queue.begin();
    while (iter != _queue.end())
    {
        std::string k = (*iter)->key;
        std::string v = (*iter)->val;

        printf("http params [%s %s] \n", k.c_str(), v.c_str());
        iter++;
    }    

    while (_queue.size())
    {
        Params*  p = _queue.at(0);
        delete p;
        _queue.erase(_queue.begin());
    }

    HttpParam tmp;
    tmp.swap(_queue);
}


//    const char * c = "HTTP/1.1 200 OK\r\n\
// Server: openresty\r\n\
// Date: Fri, 29 Apr 2016 12:50:18 GMT\r\n\
// Content-Type: text/html; charset=utf-8\r\n\
// Content-Length: 97216\r\n\
// Connection: keep-alive\r\n\
// Keep-Alive: timeout=20\r\n\
// Vary: Accept-Encoding\r\n\
// Last-Modified: Fri, 29 Apr 2016 12:40:02 GMT\r\n\
// Vary: Accept-Encoding\r\n\
// ETag: \"57235622-17bc0\"\r\n\
// Accept-Ranges: bytes";


void http_client_parse_header(char* indata, int inlen, int& content_len, int& header_len)
{
    const char * header_sep = "\r\n\r\n";
    const char* header_param_sep = "\r\n";
    char * ptr_pos = strstr(indata, header_sep);

    if (ptr_pos == NULL)
    {
        printf("error not found! header_sep\n");
        return;
    }

    // find http response header
    ptr_pos[0] = 0x0;
    printf("%s\n", indata);
    header_len = strlen(indata) + strlen(header_sep);

    content_len = get_http_content_len(indata);

    parse_http_param(indata, strlen(indata));
}


int get_http_content_len(char* http_response_header)
{
    int len = strlen(http_response_header);
    int i = 0;
    const char* sep = "\r\n";
    const char* content_len = "Content-Length";

    int icontent_len = 0;
    int num = 0;


    while (i < len)
    {
        char * ptr = strstr(http_response_header + i, sep);

        if (ptr == NULL) {
            // not found!
            printf("not found!\n");
            break;
        } else {
            char buff[1024] = {0};
            int offset = ptr - (http_response_header + i);
            memcpy(buff, http_response_header + i, offset);
            i += offset + strlen(sep);

            printf("%s\n", buff);

            char* c = strstr(buff, content_len);
            if (c != NULL) {
                // find
                
                char* ipos = NULL;
                int bfound_sep = 0;
                for (int i = 0; i < offset; ++i)
                {
                    if (buff[i] == ':')
                    {
                        bfound_sep = 1;
                    }

                    if (bfound_sep && buff[i] > '0' && buff[i] <= '9')
                    {
                        ipos = buff + i;
                        printf("found content len %s\n", ipos);
                        icontent_len = atoi(ipos);
                        break;
                    }
                }
                
                printf("i found [%s %d]\n", content_len, icontent_len);
                break;
            }
        }
    }

    return icontent_len;
}


int main()
{
#if 0
    const char* url = "www.csdn.net";
   
    int fd = http_client_dns_resolve(url);
    
    if (fd > 0) {
        http_client_sendmessage(fd, url);
    }
#endif

#if 1
    const char * c = "HTTP/1.1 200 OK\r\n\
Server: openresty\r\n\
Date: Fri, 29 Apr 2016 12:50:18 GMT\r\n\
Content-Type: text/html; charset=utf-8\r\n\
Content-Length: 97216\r\n\
Connection: keep-alive\r\n\
Keep-Alive: timeout=20\r\n\
Vary: Accept-Encoding\r\n\
Last-Modified: Fri, 29 Apr 2016 12:40:02 GMT\r\n\
Vary: Accept-Encoding\r\n\
ETag: \"57235622-17bc0\"\r\n\
Accept-Ranges: bytes";

    char header_buff[2048] = {0};
    sprintf(header_buff, "%s", c);

    parse_http_param(header_buff, strlen(header_buff));
#endif
    return 0;
}







