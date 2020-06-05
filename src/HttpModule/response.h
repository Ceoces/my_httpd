#ifndef RESPONSE_H
#define RESPONSE_H
#include<map>
#include<string>
#include<iostream>

#include<unistd.h>

#include "../util/util.h"

using std::map;
using std::string;

#define DEFAULT_HTTP_VERSION 11

class Response
{
private:
    /* data */
    short _version;
    int _status_code;
    string _status_info;
    map<string,string> _field;
private:
    void intiField();

public:
    Response(int status_code);
    Response &setVersion(short version);
    Response &setStatus(int status_code, string status_info);
    Response &setField(string key, string val);
    void sendHeader(int fd);
    ~Response();
};

inline string code_to_info(int code);
#endif