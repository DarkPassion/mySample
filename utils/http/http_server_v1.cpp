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
// Multipart/form-data
// http://blog.csdn.net/MSPinyin/article/details/6141638


int mylistener(int port);

void on_connect(int fd, struct sockaddr* client_addr);

void on_client_send_message(int fd);

//  直接用于下载文件
void on_client_download_file(int fd);

// chunked 
void on_client_chunked(int fd);

void set_socket_noblocking(int fd);

void on_client_recvmsg(int fd);

void set_socket_reuseable(int fd);

int get_http_content_len(char* http_response_header);

void http_client_parse_header(char* indata, int inlen, int& content_len, int& header_len);


// fix handle http client upload file
void http_write_upload_file(FILE* file, char* idnata, int inlen, int isheader);

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
	on_client_recvmsg(fd);

	//on_client_send_message(fd);
	//on_client_download_file(fd);
	on_client_chunked(fd);

	close(fd);
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

}

void http_write_upload_file(FILE* file, char* indata, int inlen, int isheader)
{

	if (file == NULL || indata == NULL || inlen <= 0)
	{
		printf("http_write_upload_file error !\n");
		return;
	}

	const char* sep = "\r\n";
	const char* sepsep = "\r\n\r\n";

	char* boundary = NULL;
	char* content_pos = NULL;
	char* content_type = NULL;

	if (isheader)
	{
		int ipos = 0;
		int i = 0;
		int skip_len = 0;
		while(i < inlen)
		{
			if (indata[i + 0] == sep[0] &&
				indata[i + 1] == sep[1])
			{
				int len = i - ipos;
				char line[1024] = {0};
				memcpy(line, indata + ipos, len);
				printf("line %s\n", line);

				if (true)
				{
					int j=0;
					const char* b = "--";
					const char* cd = "Content-Disposition";
					const char* ct = "Content-Type";
					while (j < strlen(line))
					{
						if (true)
						{
							const char* c = strstr(line, b);
							if (c)
							{
								boundary = new char[1024];
								memset(boundary, 0, 1024);
								memcpy(boundary, line, strlen(line));
								skip_len += strlen(line) + 2;
								break;
							}
						}

						if (true)
						{
							const char* c=  strstr(line, cd);
							if (c)
							{
								content_pos = new char[1024];
								memset(content_pos, 0, 1024);
								memcpy(content_pos, line, strlen(line));
								skip_len += strlen(line) + 2;
								break;
							}

						}

						if (true)
						{
							const char* c = strstr(line, ct);
							if (c)
							{
								content_type = new char[1024];
								memset(content_type, 0, 1024);
								memcpy(content_type, line, strlen(line));
								skip_len += strlen(line) + 2;
								break;
							}
						}
					} // end while

				} // end if

				ipos = i + 2;
				i += 2;
			} else {
				i++;
			}
		} // end while

		fwrite(indata + skip_len, inlen - skip_len, 1, file);

		if (boundary)
		{
			printf("%s\n", boundary);
			delete[] boundary;
		}

		if (content_type)
		{
			printf("%s\n", content_type);
			delete[] content_type;
		}

		if (content_pos)
		{
			printf("%s\n", content_pos);
			delete[] content_pos;
		}


	} else {

		const char* boundary = "----WebKitFormBoundaryrGKCBY7qhFd3TrwA";

		const char * c = strstr(indata, boundary);
		if (c == NULL)
		{
			fwrite(indata, inlen, 1, file);
		} else {
			const int skip_len = strlen(boundary) + 2;
			fwrite(indata, inlen - skip_len, 1, file);
		}

	}

}

void on_client_recvmsg(int fd)
{
	int recvlen = 0;
    int content_len = 0;
    int header_len = 0;

    FILE* _file = fopen("upload.cpp", "wb");
    while (1) {

        fd_set fds;
        int n;
    
        // 10s timeout
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
            http_write_upload_file(_file, outbuff + header_len, nrecv, 1);
            //printf("%s\n", outbuff);
        } else {
        	http_write_upload_file(_file, outbuff, nrecv, 0);
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
    fclose(_file);
    _file = NULL;
    printf("all recv len [%d]\n", recvlen);
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



