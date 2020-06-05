#ifndef THREADPOOL_H
#define  THREADPOOL_H

#include "WorkerThread.h"

#include "../log/logger.h"
#include "netThreadPoolInterface.h"

#include<iostream>
#include <thread>
#include <vector>

using std::vector;
using std::thread;

class WorkerThread;

class ThreadPool : public netThreadPoolInterface
{
private:
	int init_size;
	vector<WorkerThread *> workerList;

public:
	ThreadPool(int size = 4);
	~ThreadPool();

	void start();
	bool addTask(int fdTask);
	bool delConn(int fd);
	
};

#endif