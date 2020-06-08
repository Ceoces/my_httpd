#ifndef MASTERTHREAD_H
#define  MASTERTHREAD_H

#include<vector>
#include<iostream>
#include <cstdlib>
#include <cstring>

#include<mutex>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "../util/util.h"
#include "../log/logger.h"
#include "ThreadPool.h"


using std::vector;

#define DEFAULT_ADDRESS "0.0.0.0"
#define DEFAULT_PORT 2019
#define DEFAULT_MAXCONN 50

#include "netThreadPoolInterface.h"

class MasterThread
{
private:
	int fdServer = -1;
	int maxConn = DEFAULT_MAXCONN;
	struct sockaddr_in ServerAddr;
	bool setAddr(int Port, std::string Address);
	bool stopFlag = false;
public:
	ThreadPool *threadPool = nullptr;
public:
	MasterThread(int Port, std::string Address);
	void setMaxConn(int n);
	void bindPool(ThreadPool *p);
	int startListen();
	~MasterThread();
	void run();
	void stop();
};

#endif