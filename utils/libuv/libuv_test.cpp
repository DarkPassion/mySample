
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128

// gcc -lstdc++ libuv_test.cpp -I/usr/local/include -L/usr/local/lib -luv


uv_loop_t *loop;

// uv tcp server api
void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);

void server_echo_write(uv_write_t *req, int status);

void server_echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);

void on_new_connection(uv_stream_t *server, int status);

void run_tcp_server(uv_tcp_t* server);

// uv tcp client api

void client_echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);

void client_echo_write(uv_write_t *req, int status);

void on_connected(uv_connect_t* req, int status);

void run_tcp_client(uv_tcp_t* client);

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void server_echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free(req);
}

void server_echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        fprintf(stderr, "tcp server read error nread < 0 ! \n ");
        uv_close((uv_handle_t*) client, NULL);
    } else if (nread > 0) {
        fprintf(stdout, "server read %.*s \n", nread, buf->base);
        
        uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
        uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
        uv_write(req, client, &wrbuf, 1, server_echo_write);
    }
    
    if (buf->base)
        free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status) {
    printf("on new connection \n");
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }
    
    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        struct sockaddr_in in;
        int inlen = sizeof(in);
        uv_tcp_getpeername(client, (struct sockaddr*)&in, &inlen);
        char* ip = inet_ntoa(in.sin_addr);
        int port = ntohs(in.sin_port);
        printf("uv accept client  %s %d ! \n", ip, port);
        uv_read_start((uv_stream_t*) client, alloc_buffer, server_echo_read);
    }
    else {
        uv_close((uv_handle_t*) client, NULL);
    }
}

void run_tcp_server(uv_tcp_t* server)
{
    struct sockaddr_in addr;
    
    uv_tcp_init(loop, server);
    
    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);
    
    uv_tcp_bind(server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*) server, DEFAULT_BACKLOG, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return;
    }
    
}


void run_tcp_client(uv_tcp_t* client)
{
    
    struct sockaddr_in addr;
    uv_tcp_init(loop, client);
    
    uv_ip4_addr("127.0.0.1", DEFAULT_PORT, &addr);
    
    uv_connect_t* creq = (uv_connect_t*)malloc(sizeof(uv_connect_t));
    
    uv_tcp_connect(creq, client, (const struct sockaddr*)&addr, on_connected);
    
}

void on_connected(uv_connect_t* req, int status)
{
    if (status) {
        fprintf(stderr, "Connect error %s\n", uv_strerror(status));
        free(req);
        return;
    }
    
    
    uv_write_t *wreq = (uv_write_t *) malloc(sizeof(uv_write_t));
    char msg[] = "hello world !";
    uv_buf_t wrbuf = uv_buf_init(msg, sizeof(msg));
    uv_write(wreq, (uv_stream_t*)req->handle, &wrbuf, 1, client_echo_write);
    
    uv_read_start((uv_stream_t*) req->handle, alloc_buffer, client_echo_read);
    
    free(req);
    
}



void client_echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free(req);
}

void client_echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        fprintf(stderr, "tcp server read error nread < 0 ! \n ");
        uv_close((uv_handle_t*) client, NULL);
    } else if (nread > 0) {
        fprintf(stdout, "client read : %.*s \n", nread, buf->base);
        
        uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
        uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
        uv_write(req, client, &wrbuf, 1, client_echo_write);
    }
    
    if (buf->base)
        free(buf->base);
}


int main() {
    loop = uv_default_loop();
    
    uv_tcp_t server;
    run_tcp_server(&server);
    
    
    uv_tcp_t client;
    run_tcp_client(&client);
    
    uv_run(loop, UV_RUN_DEFAULT);
    
    
    return 0;
}
