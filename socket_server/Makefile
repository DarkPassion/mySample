
RM = rm -f

default : all

all : socket-test http-server http-client

socket-test : socket_server.c test.c
	gcc -g -Wall -o $@ $^ -lpthread

http-server : socket_server.c http_server.c
	gcc -g -Wall -o $@ $^ -lpthread

http-client : socket_server.c http_client.c
	gcc -g -Wall -o $@ $^ -lpthread


clean:
	$(RM) socket-test
	$(RM) http-server
	$(RM) http-client
