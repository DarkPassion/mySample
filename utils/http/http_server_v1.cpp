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


// g++ http_server_v1.cpp -o http_server_v1.out



int mylistener(int port);

void on_connect(int fd, struct sockaddr* client_addr);

void set_socket_noblocking(int fd);


void set_socket_reuseable(int fd);

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


void set_socket_reuseable(int fd)
{
	int one = 1;
	/* REUSEADDR on Unix means, "don't hang on to this address after the
	 * listener is closed."  On Windows, though, it means "don't keep other
	 * processes from binding to this address while we're using it. */
	socklen_t len = sizeof(one);
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*) &one, len);
}

void on_connect(int fd, struct sockaddr* client_addr)
{
	printf("on_connect \n");
	struct sockaddr_in * in = (struct sockaddr_in*) client_addr;
	const char * ip = inet_ntoa(in->sin_addr);
	int port = ntohs(in->sin_port);
	printf("client ip [%s %d]\n", ip, port);

	// set_socket_noblocking(fd);

	char request[2048] = {0};
	int nrecv = recv(fd, request, sizeof(request), 0);
	if (nrecv < 0)
	{
		printf("recv error !\n");
		return;
	}

	printf("on_connect request [%s]\n", request);


	const char* message = "hello world!";
	const char* sep = "\r\n";
	int len = strlen(message);
	std::ostringstream ss;
	ss << "HTTP/1.1 200 OK" << sep;
	ss << "Server: openresty" << sep;
	ss << "Content-Type: text/html; charset=utf-8" << sep;
	ss << "Content-Length: " << len << sep;
	ss << "Connection: keep-alive" << sep << sep;
	ss << message;

	std::string out = ss.str();
	int outlen = strlen(out.c_str());

	int nsend = send(fd, out.c_str(), outlen, 0);

	printf("on connect send byte [%d] \n", nsend);
	close(fd);
}


int mylistener(int port)
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

	ret = listen(fd, 10);
	if (ret < 0)
	{
		printf("listen error!\n");
		return -1;
	}

	while (1)
	{
		printf("begin accept\n");
		struct sockaddr_in client_addr;
		socklen_t len = sizeof(client_addr);
		memset(&client_addr, 0, sizeof(client_addr));
		int clientfd = accept(fd, (struct sockaddr*)&client_addr, &len);
		if (clientfd) {
			on_connect(clientfd, (struct sockaddr*)&client_addr);
		} else {
			printf("accept error !\n");
		}
	}
	


	return 0;
}

int main ()
{

	mylistener(80);

	return 0;
}



