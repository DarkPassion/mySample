/**
 * C time format utils
 *
*/

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <assert.h>


std::string int_str(int i)
{
    std::string str;
    char buff[10] = {0};
    sprintf(buff, "%d", i);
    str = buff;
    return str;
}

char* local_time_str()
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char* time_str = asctime(timeinfo);
    *(time_str + strlen(time_str) -1) = '\0'; // remove '\n'
    return time_str;
}

struct tm* get_local_time()
{
    time_t tt;
    time(&tt);
    struct tm * p;
    p = localtime(&tt);
    
    // printf("h: %d m: %d  s: %d\n", p->tm_hour, p->tm_min, p->tm_sec);
    return p;
}


std::string get_cur_time()
{
    tm* p = get_local_time();

    std::string s;
    s += int_str(p->tm_year + 1900);
    s += int_str(p->tm_mon + 1);
    s += int_str(p->tm_mday);
    return s;
}

std::string get_cur_sec()
{
    tm* p = get_local_time();
    std::string hour = p->tm_hour > 9 ? int_str(p->tm_hour) : ("0" + int_str(p->tm_hour));
    std::string min = p->tm_min > 9 ? int_str(p->tm_min) : ("0" + int_str(p->tm_min));
    std::string sec = p->tm_sec > 9 ? int_str(p->tm_sec) : ("0" + int_str(p->tm_sec));
   
    printf("h : %s m : %s s : %s\n", hour.c_str(), min.c_str(), sec.c_str());
    return hour + ":" + min + ":" +  sec;
}



int main()
{
    const char * p ="101";
    std::string i = int_str(101);
    assert(strcmp(p, i.c_str()) == 0); 

    std::string s = get_cur_sec();

    printf("get_cur_sec %s\n", s.c_str());

    std::string cur_time = get_cur_time();
    printf("get_cur_time %s\n", cur_time.c_str());


	time_t time_t1;

	time(&time_t1);

	printf("time %u", time_t1);

	return 0;
}








