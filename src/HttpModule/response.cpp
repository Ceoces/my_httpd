#include "response.h"

//TODO response构造函数
Response::Response(int status_code) : _status_code(status_code),
    _status_info(code_to_info(status_code)), 
    _version(DEFAULT_HTTP_VERSION)
{
    intiField();;
}
//TODO 设置version
Response &Response::setVersion(short version){
    _version = version;
    return *this;
}
//TODO 设置响应报文状态
Response &Response::setStatus(int status_code, string status_info){
    _status_code = status_code;
    _status_info = status_info;
    return *this;
}
//TODO 设置报文属性
Response &Response::setField(string key, string val)
{
    _field[key] = val;
    return *this;
}

void Response::intiField(){
    _field["Server"] = "httpd";
    _field["Date"] = util::getTime();
    _field["Content-Length"] = "0";
}

//TODO 发送报文头
void Response::sendHeader(int fd){
    string header = "HTTP/";
    header += std::to_string(_version/10%10);
    header += '.';
    header +=  std::to_string(_version%10);
    header += ' ';
    header +=  std::to_string(_status_code);
    header += ' ';
    header += _status_info;
    header += "\r\n";
    
    for(auto &it : _field){
        header += it.first;
        header += ": ";
        header += it.second;
        header += "\r\n";
    }
    header += "\r\n";
    std::cout << header <<std::endl;
    write(fd, header.c_str(), header.length());
}
Response::~Response()
{
}

inline string code_to_info(int code)
{
    switch (code)
    {
    case 200:
        return "OK";
    case 404:
        return "Not Found";
    case 400:
        return "Bad Request";
    case 500:
        return "Internal Server Error";
    case 403:
        return "Forbidden";

    default:
        return "";
    }
}