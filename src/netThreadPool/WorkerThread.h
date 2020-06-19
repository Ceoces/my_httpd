#ifndef WORKERTHREAD_H
#define  WORKERTHREAD_H

#include<vector>
#include<iostream>
#include <cstdlib>
#include <cstring>

#include<mutex>
#include <thread>
#include <atomic>

using std::vector;
using std::thread;

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "../HttpModule/request.h"
#include "../ioThreadPool/ioThreadPoolInterface.h"
#include "../ioThreadPool/ioWorker.h"
#include "../ioThreadPool/ioPool.h"
#include "ThreadPool.h"

#include "../log/logger.h"

class ThreadPool;

class WorkerThread
{
private:
	int no;
	bool stopFlag = false;
	thread *worker;
	int maxClient = 50;
	int fdEpoll = -1;
	
public:
	struct epoll_event *evList;
	
	std::atomic<int> ClientNum ;

	WorkerThread();
	WorkerThread(int m_no) : no(m_no){}

	int removeConn(int fd);
	
	
	void stop();
	int addTask(int fdTask);

	friend void do_work(WorkerThread *wt);

	~WorkerThread();
};

void do_work(WorkerThread *wt);

#endif