#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


using namespace std;


int create_listener();

void make_no_pipe();

void set_socket_reuseable(int fd);

void set_socket_noblocking(int fd);

void test_http_client();


void set_socket_reuseable(int fd)
{
    int one = 1;
    /* REUSEADDR on Unix means, "don't hang on to this address after the
     * listener is closed."  On Windows, though, it means "don't keep other
     * processes from binding to this address while we're using it. */
    socklen_t len = sizeof(one);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*) &one, len);
}




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

int create_listener()
{

    int port = 10801;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    //addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_addr.s_addr = INADDR_ANY;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("socket error!\n");
        return -1;
    }

    set_socket_reuseable(fd);
    int ret = ::bind(fd, (struct sockaddr *) &addr, sizeof(addr));
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

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        memset(&client_addr, 0, sizeof(client_addr));
        int clientfd = ::accept(fd, (struct sockaddr *) &client_addr, &len);
        if (clientfd > 0)
        {
            printf("on_accept [%d %s]\n", clientfd, inet_ntoa(client_addr.sin_addr));

            char buff[256] = {0};
            snprintf(buff, sizeof(buff) - 1, "%s", "hello!");

            int nsend = send(clientfd, buff, strlen(buff), 0);

            printf("client socket send [%d %s] \n", nsend, buff);
            close(clientfd);

        }
        else
        {
            printf("accept error !\n");
            return -1;
        }

    }

}


void make_no_pipe()
{
    struct sigaction sigact;

    /* Ignore SIGPIPE */
    sigact.sa_handler = SIG_IGN;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;

    if (sigaction(SIGPIPE, &sigact, NULL) < 0)
    {
        printf("SIGPIPE SIG_ERR install fail!\n");
    }
}


#include "HttpClient.h"
void test_http_client()
{
    HttpClient client;
    // Upgrade-Insecure-Requests: 1

    client.add_params("Upgrade-Insecure-Requests", "1");
    client.init_url(0, "http://geek.csdn.net/news/detail/73610");
    client.get_response();
}

int main()
{
    cout << "Hello, World!" << endl;

    make_no_pipe();
//    create_listener();

    test_http_client();

    return 0;
}

