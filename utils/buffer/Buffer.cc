


#include <errno.h>
#include <string.h>
#include <sys/uio.h>

#include "Buffer.h"


const char Buffer::kCRLF[] = "\r\n";


void Buffer::appendInt32(int32_t x)
{
    int32_t be32 = x;//sockets::hostToNetwork32(x);
    append(&be32, sizeof be32);
}

void Buffer::appendInt16(int16_t x)
{
    int16_t be16 = x; //sockets::hostToNetwork16(x);
    append(&be16, sizeof be16);
}

int32_t Buffer::peekInt32() const
{
    int32_t be32 = 0;
    ::memcpy(&be32, peek(), sizeof be32);
    return be32;
    //return sockets::networkToHost32(be32);
}

int16_t Buffer::peekInt16() const
{
    int16_t be16 = 0;
    ::memcpy(&be16, peek(), sizeof be16);
    return be16;
    // return sockets::networkToHost16(be16);
}


