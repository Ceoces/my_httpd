#ifndef IOWORKER_H
#define IOWORKER_H

#include<queue>
#include<string>

#include<thread>
#include<mutex>
#include<condition_variable>

#include <unistd.h>
#include <arpa/inet.h>

#include<iostream>

#include "../log/logger.h"

#include "../HttpModule/request.h"
#include "../HttpModule/response.h"
#include "../netThreadPool/netThreadPoolInterface.h"
#include "ioThreadPoolInterface.h"

using std::queue;
using std::string;

class ioWorker
{
private:
    /* data */
    bool _stop = false;
    std::thread *_worker;
    int _no;
private:
    void do_task(ioTask task);
public:
    ioWorker(int no);
    ~ioWorker();
    void working( );
    void start();
    void stop();
};

#endif
