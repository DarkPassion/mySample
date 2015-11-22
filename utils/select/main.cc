#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>

#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#define STDIN 0 


// gcc main.cc -o main.out

int recv_timeout(int s, char* buff, int len, int timeout);

int recv_timeout(int s, char* buff, int len, int timeout)
{
    fd_set fds;
    int n;

    struct timeval tv;

    FD_ZERO(&fds);
    FD_SET(s, &fds);

    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    // wait until timeout or data recevied
    
    n = select(s + 1, &fds, NULL, NULL, &tv);
    if (n ==0)  return -2;  // timeout
    if (n == -1) return -1; // error
    
    // data must be here, so do a noraml recv

    return recv(s, buff, len, 0);
}



/*
** packi16() -- store a 16-bit int into a char buffer (like htons())
*/ 
void packi16(unsigned char *buf, unsigned int i)
{
    *buf++ = i>>8; *buf++ = i;
}

/*
** packi32() -- store a 32-bit int into a char buffer (like htonl())
*/ 
void packi32(unsigned char *buf, unsigned long int i)
{
    *buf++ = i>>24; *buf++ = i>>16;
    *buf++ = i>>8;  *buf++ = i;
}

/*
** packi64() -- store a 64-bit int into a char buffer (like htonl())
*/ 
void packi64(unsigned char *buf, unsigned long long int i)
{
    *buf++ = i>>56; *buf++ = i>>48;
    *buf++ = i>>40; *buf++ = i>>32;
    *buf++ = i>>24; *buf++ = i>>16;
    *buf++ = i>>8;  *buf++ = i;
}

/*
** unpacki16() -- unpack a 16-bit int from a char buffer (like ntohs())
*/ 
int unpacki16(unsigned char *buf)
{
    unsigned int i2 = ((unsigned int)buf[0]<<8) | buf[1];
    int i;

    // change unsigned numbers to signed
    if (i2 <= 0x7fffu) { i = i2; }
    else { i = -1 - (unsigned int)(0xffffu - i2); }

    return i;
}

/*
** unpacku16() -- unpack a 16-bit unsigned from a char buffer (like ntohs())
*/ 
unsigned int unpacku16(unsigned char *buf)
{
    return ((unsigned int)buf[0]<<8) | buf[1];
}

/*
** unpacki32() -- unpack a 32-bit int from a char buffer (like ntohl())
*/ 
long int unpacki32(unsigned char *buf)
{
    unsigned long int i2 = ((unsigned long int)buf[0]<<24) |
                           ((unsigned long int)buf[1]<<16) |
                           ((unsigned long int)buf[2]<<8)  |
                           buf[3];
    long int i;

    // change unsigned numbers to signed
    if (i2 <= 0x7fffffffu) { i = i2; }
    else { i = -1 - (long int)(0xffffffffu - i2); }

    return i;
}

/*
** unpacku32() -- unpack a 32-bit unsigned from a char buffer (like ntohl())
*/ 
unsigned long int unpacku32(unsigned char *buf)
{
    return ((unsigned long int)buf[0]<<24) |
           ((unsigned long int)buf[1]<<16) |
           ((unsigned long int)buf[2]<<8)  |
           buf[3];
}

/*
** unpacki64() -- unpack a 64-bit int from a char buffer (like ntohl())
*/ 
long long int unpacki64(unsigned char *buf)
{
    unsigned long long int i2 = ((unsigned long long int)buf[0]<<56) |
                                ((unsigned long long int)buf[1]<<48) |
                                ((unsigned long long int)buf[2]<<40) |
                                ((unsigned long long int)buf[3]<<32) |
                                ((unsigned long long int)buf[4]<<24) |
                                ((unsigned long long int)buf[5]<<16) |
                                ((unsigned long long int)buf[6]<<8)  |
                                buf[7];
    long long int i;

    // change unsigned numbers to signed
    if (i2 <= 0x7fffffffffffffffu) { i = i2; }
    else { i = -1 -(long long int)(0xffffffffffffffffu - i2); }

    return i;
}

/*
** unpacku64() -- unpack a 64-bit unsigned from a char buffer (like ntohl())
*/ 
unsigned long long int unpacku64(unsigned char *buf)
{
    return ((unsigned long long int)buf[0]<<56) |
           ((unsigned long long int)buf[1]<<48) |
           ((unsigned long long int)buf[2]<<40) |
           ((unsigned long long int)buf[3]<<32) |
           ((unsigned long long int)buf[4]<<24) |
           ((unsigned long long int)buf[5]<<16) |
           ((unsigned long long int)buf[6]<<8)  |
           buf[7];
}


int main()
{

    struct timeval tv;
    fd_set readfd;


    tv.tv_sec = 2;
    tv.tv_usec = 0;


    FD_ZERO(&readfd);
    FD_SET(STDIN, &readfd);


    select(STDIN + 1, &readfd, NULL, NULL, &tv);

    if (FD_ISSET(STDIN, &readfd)) {
        printf("a key was prassed!\n");
        
        char buff[256] = {0};
        int n_read = read(STDIN, buff, sizeof(buff));

        printf("read -- n %d buff %s\n", n_read, buff);
    } else {
        printf("Time out.\n");
    }


    unsigned char buff[256] = {0};
    packi16(buff, 123);

    printf("packi16 -- %d \n", unpacku16(buff));

    memset(buff, 0, sizeof(buff));
    packi32(buff, 0x11223344);
    printf("packet32 -- %lx\n", unpacku32(buff));
    memset(buff, 0, sizeof(buff));


    packi64(buff, 0x112233445566);
    printf("packi64 -- %llx\n", unpacku64(buff));
    memset(buff, 0, sizeof(buff));



    
    return 0;
}
