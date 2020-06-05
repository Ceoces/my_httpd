#ifndef IO_THREAD_POOL_INTERFACE
#define IO_THREAD_POOL_INTERFACE

#include<queue>
#include<mutex>
#include<string>
#include<condition_variable>

//TODO IO任务
struct ioTask{
    //任务类型
    enum taskTpye{
        READ_FROM_SOCK,
        WRITE_TO_SOCK,
        FILE_TO_SOCK
    } type;

    //任务资源句柄和指针
    union __taskResource
    {
        struct _sock_fd{
            int sockfd;
            int filefd;
        }sock_fd;
        struct _sock_buf{
            int sockfd;
            std::string *buf;
        }sock_buf;

    }taskResource;
};

extern std::queue<ioTask> ioTaskQueue;
extern std::mutex ioPoolmcv;
extern std::condition_variable ioPoolcv;
extern  int TaskNum;


#endif