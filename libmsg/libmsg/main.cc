#include <assert.h>
#include <stdlib.h>
#include <WinSock2.h>
#include "Msg.h"

#pragma comment(lib, "ws2_32.lib")

void on_message(Message* msg)
{
	printf("msg type[%x] len [%d] data[%s]\n", msg->type, msg->len, msg->data);
}

void socket_server();

void socket_client();



void socket_server()
{
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(1990);
	sin.sin_addr.s_addr = inet_addr("0.0.0.0");

	int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (bind(sfd, (struct sockaddr*)&sin, sizeof(sin)) != 0) {

		printf("bind error !\n");
	}

	if (listen(sfd, 10) != 0) {
		printf("listen error !\n");
	}

	printf("wait for client connection !\n");
	while(true) {
		struct sockaddr_in cli_addr;
		int len = sizeof(cli_addr);
		int cli_fd = accept(sfd, (struct sockaddr*)&cli_addr, &len);

		if (cli_fd == INVALID_SOCKET) {
			continue;
		}

		char* ip = inet_ntoa(cli_addr.sin_addr);
		int port = cli_addr.sin_port;

		printf("client connection ip %s port %d\n", ip, port);

		char send_buff[256] = {0};
		const char* content = "bye bye !";
		EncodeMsg::parse_message(12345, strlen(content), content, send_buff);

		send(cli_fd, send_buff, strlen(send_buff + 8) + 8, 0);
		closesocket(cli_fd);
	}

}

void socket_client()
{
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(1990);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (sfd == INVALID_SOCKET) {
		printf("INVALLD SOCKET --- \n");
	}
	
	int re_try = 5;

	while(re_try-- > 0) {
		if (connect(sfd, (struct sockaddr*)&sin, sizeof(sin)) != 0) {
			printf("connect error !\n");
			Sleep(50);
		} else {
			printf("connect succect !\n");
			break;
		}
	}
	
	
	DecodeMsg de;
	char read_buff[256] = {0};
	while(true) {

		memset(read_buff, 0, sizeof(read_buff));
		int n_recv = recv(sfd, read_buff, sizeof(read_buff), 0);
		if (n_recv <= 0) {
			break;
		}

		de.parse_data(n_recv, read_buff);

	}

	closesocket(sfd);
	printf("client socket end!\n");
	
}




int main()
{
	WSADATA  Ws;
	WSAStartup(MAKEWORD(2,2), &Ws);

	HANDLE hs = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)socket_server, NULL, 0, NULL);

	HANDLE hc = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)socket_client, NULL, 0, NULL);


	char buff[256] = {0};
	
	char msg_array[100] = {0};

	sprintf(msg_array, "helloworld");
	EncodeMsg::parse_message(0x123456, strlen(msg_array), msg_array, buff);

	DecodeMsg de;
	for (int i = 0; i < strlen(msg_array) + 8; i++)
	{
		de.parse_data(1, &buff[i]);
	}
	
	memset(buff, 0, sizeof(buff));
	utility::write_int16(buff, 0x123);
	int n_read = utility::read_int16(buff);
	assert(n_read == 0x123);
	memset(buff, 0, sizeof(buff));
	
	utility::write_int16(buff, -123);
	n_read = utility::read_int16(buff);
	assert(n_read == -123);
	memset(buff, 0, sizeof(buff));

	AVMsg avmsg;
	memset(&avmsg, 0, sizeof(avmsg));
	char temp_buff[256] = "libmsg test console!";
	avmsg.data = temp_buff;
	avmsg.len = strlen(temp_buff);
	utility::write_vamsg(buff, &avmsg);

	int len = utility::read_vamsg(buff);
	AVMsg lmsg;
	lmsg.data = new char[len];
	lmsg.len = len;
	utility::read_vamsg(buff, lmsg);
	delete[] lmsg.data;

	
	memset(buff, 0, sizeof(buff));
	utility::write_int64(buff, 0x112233445566);
	int64_t r64 = utility::read_int64(buff);
	assert(r64 == 0x112233445566);


	gets(buff);

	CloseHandle(hs);
	CloseHandle(hc);
	return 0;
}