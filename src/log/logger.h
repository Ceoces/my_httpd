/*
 * @Author: ZhaoEnBo
 * @Date: 2020-05-20 15:27:25 
 * @Last Modified by: zhaoenbo
 * @Last Modified time: 2020-05-20 17:16:06
 */
#ifndef LOGGER_H
#define LOGGER_H

#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>

#include<mutex>
#include<thread>

#include<sys/time.h>
#include<unistd.h>

#define ZLOG_INIT_SIZE 5

#define LOGERR ZLog::_ZLOG(ZLog::ZLogLeval::ERROR, __FILE__, __LINE__)
#define LOGINFO ZLog::_ZLOG(ZLog::ZLogLeval::INFO, __FILE__, __LINE__)
#define LOGWARN ZLog::_ZLOG(ZLog::ZLogLeval::WARRING, __FILE__, __LINE__)
#define LOGSTART(x) ZLog::SampleLogger::get()->start(x)
#define LOGSTOP() ZLog::SampleLogger::get()->stop()

namespace ZLog{

     enum ZLogLeval {
            INFO,
            WARRING,
            ERROR
        };
    extern char *ZLogLevalMsg[];


    class Logger
    {
    private:
        int _size;
        int now;
        std::vector<std::string> *_buf[2];
        std::vector<std::mutex> *_mbuf[2];
        bool _stop = false;
    private:
        int _hash();
    public:
        Logger(int size) ;  
        void add(std::string &info);
        void logging(std::string logname);
        void start(std::string logname);
        void stop();
        ~Logger();
    };

    //Logger单例类
    class SampleLogger{
    public:
        static Logger *_logger;
        static std::mutex m;
        static Logger *get();
    };

    inline std::string getTime(void);

    //LOG 操作类 重载<< 运算符 析构写日志
    class _ZLOG
    {
    private:
        std::string _info;
    public:
        _ZLOG(ZLogLeval leval, std::string file, int line);
        _ZLOG &operator << (std::string info);
        _ZLOG &operator << (int info);
        _ZLOG &operator << (short);
        _ZLOG &operator << (long);
        _ZLOG &operator << (long long);
        _ZLOG &operator << (char *);
        _ZLOG &operator << (float);
        _ZLOG &operator << (double);
        _ZLOG &operator << (unsigned int);
        _ZLOG &operator << (unsigned long);
        _ZLOG &operator << (unsigned long long);
        ~_ZLOG();
    };


}
#endif
