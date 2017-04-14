#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


static uint32_t make_tag(char a, char b, char c, char d)
{
    return ((uint32_t)(a)<<24) | ((uint32_t)(b)<<16) | ((uint32_t)(c)<<8) | ((uint32_t)(d));
}

static int hash_string(const char* c)
{
    int s = 0;
    int l = strlen(c);
    for (int i = 0; i < l; i++) {
        s = (s*31) + c[i];
    }
    return s;
}

class base
{
    public:
        virtual ~base()
        {

        }

        virtual const char* get_type() = 0;
        virtual const int get_tag() = 0;
};

#define BEGIN_CLASS(c, b) \
class c : public b  \
{ \
    public: \
        virtual const char* get_type() { return #c; } \
        virtual const int   get_tag() { const char* p = #c;  return hash_string(p); } \
        virtual ~c() {  }

#define END_CLASS() }



BEGIN_CLASS(parent_a, base);
END_CLASS();

BEGIN_CLASS(parent_b, base);
END_CLASS();


void test_type(base* m)
{
    if (dynamic_cast<parent_a*>(m) != NULL) {
        printf("--- type parent_a  %p %p \n", m, dynamic_cast<parent_a*>(m));
    } else if (dynamic_cast<parent_b*>(m) != NULL) {
        printf("--- type parent_b! %p %p \n", m, dynamic_cast<parent_b*>(m));
    } else {
        printf("--- type unknow! \n");
    }
}

void test_type2(base* m)
{
    const char* mtype = m->get_type();
    if (strcmp(mtype, "parent_a") == 0) {
        printf("--- type parent_a %s \n", __FUNCTION__);
    } else if (strcmp(mtype, "parent_b") == 0) {
        printf("--- type parent_b %s \n", __FUNCTION__);
    } else {
        printf("--- type unknow %s \n", __FUNCTION__);
    }
}

void test_type3(base* m)
{
    const int mtype = m->get_tag();
    const int atype = hash_string("parent_a");
    const int btype = hash_string("parent_b");

    if (mtype == atype) {
        printf("--- type parent_a %s \n", __FUNCTION__);
    } else if (mtype == btype) {
        printf("--- type parent_b %s \b", __FUNCTION__);
    } else {
        printf("--- type unknow ! %s \n", __FUNCTION__);
    }

}

int main()
{

    base* m1 = new parent_a();
    base* m2 = new parent_b();
    test_type(m1);
    test_type(m2);

    test_type2(m1);
    test_type2(m2);

    test_type3(m1);
    test_type3(m2);



    delete m1;
    delete m2;

    return 0;
}
