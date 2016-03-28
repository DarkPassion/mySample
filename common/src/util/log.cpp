#include <fstream>
#include <iostream>
#include <stdarg.h>         // va_list
#include <sys/stat.h>       // mkdir
#include <sstream>
#include <time.h>
#include "log.h"

using namespace std;

std::string log::get_cur_time()
{
    std::string s;
    time_t tt;
    time(&tt);
    struct tm* timeinfo;
    timeinfo = localtime(&tt);

    char buff[256] = {0};
    sprintf(buff, "%d_%d_%d.log", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);

    s = buff;
    return s;
}

std::string log::get_cur_sec()
{
    std::string s;
    time_t tt;
    time(&tt);
    struct tm* timeinfo;
    timeinfo = localtime(&tt);

    char buff[256] = {0};

    timeinfo->tm_hour > 9 ? sprintf(buff, "%d", timeinfo->tm_hour) : sprintf(buff, "0%d", timeinfo->tm_hour);
    s += buff;
    s += ":";
    memset(buff, 0, sizeof(buff));
    timeinfo->tm_min > 9 ? sprintf(buff, "%d", timeinfo->tm_min) : sprintf(buff, "0%d", timeinfo->tm_min);

    s += buff;
    s += ":";
    memset(buff, 0, sizeof(buff));
    timeinfo->tm_sec > 9 ? sprintf(buff, "%d", timeinfo->tm_sec) : sprintf(buff, "0%d", timeinfo->tm_sec);
    s += buff;
    return s;
}

void log::write_log(int severity, const char* msg)
{
    const char *severity_str;

    switch (severity)
    {
    case _CR_LOG_DEBUG:
        severity_str = "Debug";
        break;
    case _CR_LOG_INFO:
        severity_str = "Info ";
        break;
    case _CR_LOG_WARN:
        severity_str = "Warn ";
        break;
    case _CR_LOG_ERROR:
        severity_str = "Error";
        break;
    }

    mkdir("Log", 0755);
    string dir = "Log/" + get_cur_time();
    const char *file_name = dir.data();
    ofstream fs;
    fs.open(file_name, ios::app);
    fs << "[" << severity_str << "]"<< "[" << get_cur_sec().c_str() << "] " << msg << endl;
    fs.close();
}

void log::cr_log(int severity, const char *fmt, va_list ap)
{
    char buf[1024];
    size_t len;
    if(fmt != NULL)
    {
        int buf_len = sizeof(buf);
        vsnprintf(buf, buf_len, fmt, ap);
        buf[buf_len-1] = '\0';
    }
    else
    {
        buf[0] = '\0';
    }
    write_log(severity, buf);
}

void log::cr_debug(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_DEBUG, fmt, ap);

    va_end(ap);
}

void log::cr_info(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_INFO, fmt, ap);

    va_end(ap);
}

void log::cr_warn(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_WARN, fmt, ap);

    va_end(ap);
}

void log::cr_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_ERROR, fmt, ap);

    va_end(ap);
}
