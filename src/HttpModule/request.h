#ifndef REQUEST_H
#define REQUEST_H

#include <map>
#include <string>
#include <cstring>
#include <sstream>

#include<dirent.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/sendfile.h>

#include "response.h"
#include "../config/myConfig.h"
#include "../log/logger.h"

#define _DEBUG_

#ifdef _DEBUG_
    #include <iostream>
    using std::cout;
    using  std::endl;
#endif

using std::map;
using std::string;

// http request method
#define METHOD_NUM 9

#define METHOD_GET 0
#define METHOD_POST 1
#define METHOD_HEAD 2
#define METHOD_CONNECT 3
#define METHOD_PUT 4
#define METHOD_DELETE 5
#define METHOD_OPTIONS 6
#define METHOD_TRACE 7
#define METHOD_PATCH 8

    static char RequestMethod[METHOD_NUM][10] =  {
        "GET", "POST", "HEAD", "CONNECT", "PUT", "DELETE", "OPTIONS", "TRACE", "PATCH"
    };

class Request
{
private:
    /* data */
    short method;  //request method
    string url;     //request url
    short version;  //http version
    map<string, string> fieldList;  //key : method value : method value
    string data;    //request data

public:
    Request(char *s,int len);
    Request(int sockfd);
    const short getMethod() const { return method;}
    const string getAttribute(std::string name){ return fieldList[name]; }
    const string getUrl(){return url;}
    const short getVersion(){return version;}
    const string getData(){return data;}
    void do_request(int fd);
    ~Request();
#ifdef _DEBUG_
    void print();
#endif
};

#endif