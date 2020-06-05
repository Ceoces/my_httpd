#ifndef IOPOOL_H
#define IOPOOL_H

#include<queue>
#include<vector>
#include<string>

#include<thread>
#include<mutex>
#include<condition_variable>

using std::queue;
using std::string;

#include<iostream>

#include "../log/logger.h"

#include "ioWorker.h"

#define IO_THREAD_DEFAULT_SIZE 2

class ioWorker;

class ioPool
{
private:
    /* data */
    //线程个数
    static int _size;
    //线程列表
    static std::vector<ioWorker *> workerList;
public:
    static bool start(int size);
    static void stop();
    static void addTask(ioTask task);
};
#endif