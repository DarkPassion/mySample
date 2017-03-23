#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "Buffer.h"


int main()
{

    {

        Buffer buf;
        buf.append("hello");
        buf.append("world");
        std::string s = buf.retrieveAsString();
        std::cout << "buf.retrieveAsString " << s << std::endl;
    }

    {
        Buffer buf;
        buf.append("~@!#$%^&*()");
        std::string s = buf.retrieveAsString();
        std::cout << "buf.retrieveAsString " << s << std::endl;
    }

    {
        Buffer buf;
        buf.appendInt32(123456789);
        int32_t s = buf.readInt32();
        std::cout << "buf.readInt32 " << s << std::endl;
    }

    {
        Buffer buf;
        buf.appendInt32(123456789);
        buf.prepend("hello", 5);
        std::string s (buf.peek(), 5);
        char s1[10];
        memset(s1, 0, sizeof(s1));
        buf.get(s1, 5);
        buf.retrieve(5);

        int32_t i32 = buf.readInt32();
        std::cout << "buf.peek s1 " << s1 <<  " buf.readInt32 "<< i32 << std::endl;

    }

    {
        Buffer buf;
        buf.appendPrintf("hello %s hh %d\n", "world", 1213);
        std::string s = buf.retrieveAsString();
        std::cout << "buf.retrieveAsString " << s << std::endl;
    }

    {
        Buffer buf;
        buf.appendPrintf("hello  %01025d\n", 0); // this is very long string
        std::string s = buf.retrieveAsString();
        std::cout << "buf.retrieveAsString " << s << std::endl;
    }


    return 0;
}
