//
// Created by zhangzhifan on 16/5/11.
//

#include <stdlib.h>
#include <stdio.h>
#include "HttpParser.h"



HttpParser::HttpParser()
{
    _is_complete = 0;

    _parser = new http_parser();
    _settings = new http_parser_settings();

    _settings->on_body = on_body;
    _settings->on_header_field = on_header_field;
    _settings->on_header_value = on_header_value;
    _settings->on_headers_complete = on_headers_complete;
    _settings->on_message_begin = on_message_begin;
    _settings->on_message_complete = on_message_complete;
    _settings->on_status_complete = on_status_complete;
    _settings->on_url = on_url;
}

HttpParser::~HttpParser()
{

    delete _parser;
    delete _settings;

    while(_queue.size())
    {
        http_parasm_t* param = _queue.at(0);
        delete param;
        _queue.erase(_queue.begin());
    }

}

void HttpParser::init_with(int type)
{
    http_parser_type t;
    if (type == 0)
    {
        t = HTTP_REQUEST;
    }
    else
    {
        t = HTTP_RESPONSE;
    }

    _parser->data = reinterpret_cast<void*>(this);
    http_parser_init(_parser, t);

}

int HttpParser::parser_content(const char* indata, size_t inlen)
{
    int outlen = http_parser_execute(_parser, _settings, indata, inlen);
    if (outlen != inlen) {
        printf("http parser execute error ! \n");
    }
    return _is_complete;
}

int HttpParser::on_url(http_parser *p, const char *c, size_t s)
{
    const char* tag = "HttpParser";
    printf("%s on_url \n", tag);
    return 0;
}

int HttpParser::on_message_complete(http_parser *p)
{
    const char* tag = "HttpParser";
    printf("%s on_message_complete \n", tag);
    HttpParser* parser = reinterpret_cast<HttpParser*>(p->data);
    parser->_is_complete = 1;
    return 0;
}


int HttpParser::on_header_field(http_parser *p, const char *c, size_t s)
{
    const char* tag = "HttpParser";
    printf("%s on_header_field %.*s \n", tag, (int)s, c);

    HttpParser* parser = reinterpret_cast<HttpParser*>(p->data);
    char buff[256] = {0};
    snprintf(buff, sizeof(buff) - 1, "%.*s", (int)s, c);
    parser->_header_curr_key = buff;
    return 0;
}

int HttpParser::on_header_value(http_parser *p, const char *c, size_t s)
{
    const char* tag = "HttpParser";
    printf("%s on_header_value %.*s \n", tag, (int)s, c);

    HttpParser* parser = reinterpret_cast<HttpParser*>(p->data);
    char buff[256] = {0};
    snprintf(buff, sizeof(buff) - 1, "%.*s", (int)s, c);
    if (parser->_header_curr_key.length() > 0)
    {
        http_parasm_t* param = new http_parasm_t;
        param->key = parser->_header_curr_key;
        param->val = buff;
        parser->_header_curr_key = "";
        parser->_queue.push_back(param);
    }

    return 0;
}


int HttpParser::on_message_begin(http_parser *p)
{
    const char* tag = "HttpParser";
    printf("%s on_message_begin\n", tag);
    return 0;
}

int HttpParser::on_body(http_parser *p, const char *c, size_t s)
{
    const char* tag = "HttpParser";
    printf("%s on_body \n", tag);
    return 0;
}

int HttpParser::on_status_complete(http_parser *p)
{
    const char* tag = "HttpParser";
    printf("%s on_status_complete [%d]\n", tag, p->status_code);
    return 0;
}

int HttpParser::on_headers_complete(http_parser* p)
{
    const char* tag = "HttpParser";
    printf("%s on_headers_complete \n", tag);

    printf("======= print http parser headers begin\n");

    HttpParser* parser = reinterpret_cast<HttpParser*>(p->data);
    for (int i = 0; i < parser->_queue.size(); ++i)
    {
        http_parasm_t* param = parser->_queue.at(i);
        printf(" %s = %s \n", param->key.c_str(), param->val.c_str());
    }

    printf("======= print http parser headers end \n");
    return 0;
}


