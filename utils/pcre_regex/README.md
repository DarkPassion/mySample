# README


[Regular expressions in C: examples?](http://stackoverflow.com/questions/1085083/regular-expressions-in-c-examples/)


[Pcre](http://www.pcre.org/)


 ```Bash
svn co svn://vcs.exim.org/pcre/code/trunk pcre
cd pcre
./autogen.sh
./configure --prefix=$(pwd)/../output/
make && make install

```



[pcre example](https://www.mitchr.me/SS/exampleCode/AUPG/pcre_example.c.html)


```C++

#include <pcre.h>

pcre* pcode = pcre_compile(const char* pattern, int options, const char** error, int* error_offset, const unsigned char* tableptr);


pcre_extra* pext = pcre_study(cosnt pcre* pcode, int options, const char** error);


pcre_exec(const pcre* pcode, const pcre_extra* pext, const char* subject, int subject_len, int start_offset, int options, int*ovector, int ovector_size);



```










