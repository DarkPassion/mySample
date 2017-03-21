#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


// g++ test_string_split.cc -o test_string_split.out
// http://stackoverflow.com/questions/236129/split-a-string-in-c


void string_split(const char* src, const char* split)
{
    char * last = NULL;
    char * token = NULL;
    char * src_cp = strdup(src);
    if ((token = strtok_r(src_cp, split, &last)) == NULL) {
        printf("could not match ! [%s %s] \n", src, split);
        free(src_cp);
        return ;
    }

    printf("string split token [%s] \n", token);

    while((token = strtok_r(NULL, split, &last)) != NULL) {
        printf("string split token [%s] \n", token);
    }

    free(src_cp);
}


// C++ split string


#include <string>
#include <sstream>
#include <vector>
#include <iterator>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        // if (!item.empty())
        *(result++) = item;
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

#include <iostream>
#include <sstream>
#include <string>
void split2(const char* src)
{
    std::string s(src);
    std::stringstream iss (s);

    do
    {
        std::string sub;
        iss >> sub;
        if (!sub.empty())
        std::cout << " Substring :" << sub << std::endl;

    } while(iss);

}

// C++ split string

int main()
{

    string_split("one:two::three", ":");

    typedef std::vector<std::string> VS;
    VS x = split("one:two::three", ':');

    for (VS::iterator iter = x.begin(); iter != x.end(); iter++) {
        printf("== x [%s] \n", iter->c_str());
    }

    split2("one two three four");
    return 0;
}
