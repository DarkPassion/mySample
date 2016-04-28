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


// g++ http_client_v1.cpp -o http_client_v1.out
// https://github.com/emilw/HttpClient.git

int http_client_sendmessage(int fd);

int http_clent_recvmessage(int fd);

int http_client_dns_resolve(const char * url);

void set_socket_noblocking(int fd);


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

int http_client_sendmessage(int fd)
{
    if (fd < 1) {
        return -1;
    }
    std::ostringstream ss;
    ss << "GET / HTTP/1.1\n";
    ss << "host:" << "www.csdn.net\n";
    ss << "User-Agent: curl/7.43.0\n";
    ss << "Accept: */*\n\n";
    
    std::string msg = ss.str();
    
    //set_socket_noblocking(fd);
    int len = strlen(msg.c_str());
    int nsend = send(fd, msg.c_str(), len, 0);
    
    printf("send message [%d %s]\n", len, msg.c_str());
    int recvlen = 0;
    while (1) {

        fd_set fds;
        int n;
    
        // 3 timeout
        struct timeval tv;
        tv.tv_sec = 3;
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
        
        recvlen += nrecv;
        printf("nrecv [%d]\n", nrecv);
        if (nrecv <= 0) {
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

int main()
{
    const char* url = "www.csdn.net";
   
    int fd = http_client_dns_resolve(url);
    
    if (fd > 0) {
        http_client_sendmessage(fd);
    }
    
    return 0;
}







