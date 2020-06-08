#ifndef NET_THREAD_POOL_INTERFACE_H
#define NET_THREAD_POOL_INTERFACE_H

#include<vector>
#include<mutex>

#include<netinet/in.h>

class netThreadPoolInterface
{
private:
    /* data */
public:
    virtual	void start() = 0;
	virtual bool addTask(int fdTask) = 0;
    virtual bool delConn(int fd) = 0;
	virtual void stop() = 0;
};
extern netThreadPoolInterface *pNetPoolInterface;

struct conn_info{
	sockaddr_in addr;
	int fd;
	int thread_num;
};
extern std::vector<conn_info> conn_list;
extern std::mutex m_conn_list;

#endif