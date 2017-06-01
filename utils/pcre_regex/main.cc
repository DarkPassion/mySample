#include <iostream>
#include <string>
#include "pcre.h"

using namespace std;

// g++ main.cc lib/libpcre.a -Iinclude
//


void test_regex1()
{
    const char* error;
    int e_offset = 0;
    const char * reg = "(.*)(hello)+";
    string s = "This could match... hello!";
    pcre* code = pcre_compile(reg, 0, &error, &e_offset, NULL);
    if (code == NULL) {
        cout << "pcre compile error  "<< error << endl;
        return ;
    }

    pcre_extra* pext = pcre_study(code, 0, &error);
    if (pext == NULL) {
        cout << "pcre study error " << error << endl;
        return ;
    }

    int ovect[30] = {0};
    int ret = pcre_exec(code, pext, s.data(), s.length(), 0, 0, ovect, 30);
    if (ret < 0) {
        cout << "pcre exec error " << endl;
        return ;
    }

    cout << "pcre exec success ! " << endl;
    for (int i = 0; i < ret; i++) {
        const char* substr;
        pcre_get_substring(s.data(), ovect, ret, i, &substr);
        cout << " pcre get substring Match(" << i << " /" << ret - 1 << ") substr: " << substr <<  endl;
        pcre_free_substring(substr);
    }
    pcre_free(code);
    if (pext != NULL) {
        pcre_free(pext);
    }
}

int main()
{


    cout << " pcre test " << endl;

    test_regex1();

    return 0;
}
