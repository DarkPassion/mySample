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

void on_client_send_message(int fd);

//  直接用于下载文件
void on_client_download_file(int fd);

// chunked 
void on_client_chunked(int fd);

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
		close(fd);
		return;
	}

	printf("on_connect request [%s %d]\n", request, nrecv);

	//on_client_send_message(fd);
	//on_client_download_file(fd);
	on_client_chunked(fd);

	close(fd);
}



void on_client_send_message(int fd)
{

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
}


void on_client_download_file(int fd)
{
	const char* filename = "http_client_v1.cpp";
	const char* sep = "\r\n";

	char* file_content = NULL;
	int filesize = 0;

	FILE* _file = fopen(filename, "rb");
	if (_file)
	{
		fseek(_file, 0, SEEK_END);
		filesize = ftell(_file);
		fseek(_file, 0, SEEK_SET);
		file_content = new char[filesize];
		fread(file_content, filesize, 1, _file);
		fclose(_file);
		_file = NULL;
	}

	if (file_content && filesize)
	{
		std::ostringstream ss;
		ss << "HTTP/1.1 200 OK" << sep;
		ss << "Server: openresty" << sep;
		ss << "Content-Type: application/octet-stream" << sep;
		ss << "Content-Length: " << filesize << sep;
		ss << "Connection: keep-alive" << sep;
		ss << "Content-Disposition: attachment; filename=" << filename;
		ss << sep;
		ss << sep;
		

		std::string out = ss.str();
		int outlen =  strlen(out.c_str());
		int nsend = send(fd, out.c_str(), outlen, 0);
		printf("on_client_download_file send byte [%d]\n", outlen);
		nsend = send(fd, file_content, filesize, 0);
		printf("on_client_download_file send byte [%d]\n", filesize);

	}


}

void on_client_chunked(int fd)
{

	// https://en.wikipedia.org/wiki/Chunked_transfer_encoding
	const char* message = "hello world!";
	const char* sep = "\r\n";
	int len = strlen(message);
	std::ostringstream ss;
	ss << "HTTP/1.1 200 OK" << sep;
	ss << "Server: openresty" << sep;
	ss << "Content-Type: text/plain; charset=utf-8" << sep;
	ss << "Transfer-Encoding: chunked" << sep << sep;
	//ss << "Connection: keep-alive" << sep << sep;
	
	std::string out = ss.str();
	int outlen = strlen(out.c_str());

	int nsend = send(fd, out.c_str(), outlen, 0);
	printf("on_client_chunked send byte [%d] \n", nsend);

	// [len][\r\n][content][\r\n]
	// len 十六进制表示
	if (true)
	{
		// 清空 ss
		char chunked_len[64] = {0};
		sprintf(chunked_len, "%x", (int)strlen(message));
		ss.str("");
		ss << chunked_len << sep;
		ss << message << sep;
		out = ss.str();
		outlen = strlen(out.c_str());
		nsend = send(fd, out.c_str(), outlen, 0);
		printf("on_client_chunked %s\n", out.c_str());
	}


	
	if (true)
	{
		const char * msg = " Evens! \n\n";
		char chunked_len[64] = {0};
		sprintf(chunked_len, "%x", (int)strlen(msg));
	
		// 清空 ss
		ss.str("");
		ss << chunked_len << sep;
		ss << msg << sep;
		out = ss.str();
		outlen = strlen(out.c_str());
		nsend = send(fd, out.c_str(), outlen, 0);
		printf("on_client_chunked %s\n", out.c_str());
	}


	if (true)
	{
		// 清空 ss
		ss.str("");
		ss << 0 << sep <<"" << sep;
		out = ss.str();
		outlen = strlen(out.c_str());
		nsend = send(fd, out.c_str(), outlen, 0);
		printf("on_client_chunked %s\n", out.c_str());

	}
	


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



