//
// Created by zhangzhifan on 16/5/11.
//

#ifndef CLIONTEST_HTTPPARSER_H
#define CLIONTEST_HTTPPARSER_H


#include <lib/http_parser.h>
#include <string>
#include <vector>

class HttpParser
{
public:
    struct http_parasm_t
    {
        std::string key;
        std::string val;
    };

    typedef std::vector<http_parasm_t*> http_params_queue;

public:
    HttpParser();

    ~HttpParser();

    void init_with(int type);

    int parser_content(const char* idnata, size_t inlen);

    static int on_message_begin(http_parser* p);

    static int on_url(http_parser* p, const char* c, size_t s);

    static int on_status_complete(http_parser* p);

    static int on_header_field(http_parser* p, const char* c, size_t s);

    static int on_header_value(http_parser* p, const char* c, size_t s);

    static int on_headers_complete(http_parser* p);

    static int on_body(http_parser*p, const char* c, size_t s);

    static int on_message_complete(http_parser* p);



private:
    HttpParser(const HttpParser& c);
    HttpParser& operator = (const HttpParser& c);

private:
    http_parser*            _parser;
    http_parser_settings*   _settings;
    http_params_queue       _queue;
    std::string             _header_curr_key;
    int                     _is_complete;
};


#endif //CLIONTEST_HTTPPARSER_H
