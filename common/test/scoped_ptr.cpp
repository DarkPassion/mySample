#include <stdlib.h>
#include <stdio.h>


#include "util/scoped_ptr.h"

// g++ scoped_ptr.cpp -I../include/ -L../libs/ -lcommon -o scoped_ptr.out

using namespace rtc;

void test_share_ptr_func();


struct Foo
{
    int i;
};

void test_share_ptr_func()
{
    scoped_ptr<Foo> foo(new Foo());
    
    scoped_ptr<Foo> foo1;          // No pointer managed.
    foo1.reset(new Foo());

}


int main()
{
    
    test_share_ptr_func();
    return 0;
}


