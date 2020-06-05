#include "util.h"



util::util()
{
}


util::~util()
{
}

    // TODO: 获取时间
std::string util::getTime()
{
    timeval tv;
    tm * time;
    std::stringstream ss;

    if(gettimeofday(&tv, nullptr) == -1){
        throw errno;
    }
    time = localtime(&(tv.tv_sec));
    char systime[40];
    //Tue, 02 Jun 2020 06:06:47 GMT
    strftime(systime, 40, "%c %Z, ",  time);
        
    ss << systime << (int64_t)(tv.tv_usec / 1000);
        
    return ss.str();
}

