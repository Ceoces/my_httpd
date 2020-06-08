#include "WorkerThread.h"

WorkerThread::WorkerThread()
{
}


WorkerThread::~WorkerThread()
{
}

void do_work(WorkerThread *wt)
{
	// Init epoll
	wt->evList = reinterpret_cast<epoll_event *>(malloc(sizeof(epoll_event)*(wt->maxClient + 1)));
	wt->fdEpoll = epoll_create(wt->maxClient + 1);

	std::cout << "Net thread " << wt->no << " start" <<std::endl;
	LOGINFO << "Net thread start";

	while (!wt->stopFlag)
	{
		if (wt->ClientNum == 0) {
			sleep(1);
		}
		// TODO epoll 事件循环
		int nfds = epoll_wait(wt->fdEpoll, wt->evList, wt->maxClient + 1, -1);
		for (int i = 0; i < nfds; i++) {
			if (wt->evList[i].events & EPOLLERR) {
				std::cout << "Net thread  " << wt->no << " event : {fd:"<< wt->evList[i].data.fd <<" , EPOLLERR}" <<endl;
				LOGERR << "Net thread  " << wt->no << " event : {fd:"<< wt->evList[i].data.fd <<" , EPOLLERR}";
			}
			else if (wt->evList[i].events & EPOLLHUP) {
				std::cout << "Net thread  " << wt->no << " event : {fd:"<<wt->evList[i].data.fd<<" , EPOLLHUP}" <<endl;
				LOGWARN << "Net thread  " << wt->no << " event : {fd:"<<wt->evList[i].data.fd<<" , EPOLLHUP}";
			}
			else if (wt->evList[i].events & EPOLLIN) {
				std::cout << "Net thread  " << wt->no << " event : {fd:"<<wt->evList[i].data.fd<<" , EPOLLIN}" <<endl;
				LOGWARN << "Net thread  " << wt->no << " event : {fd:"<<wt->evList[i].data.fd<<" , EPOLLIN}";

				ioTask task;
				task.type = ioTask::READ_FROM_SOCK;
				task.taskResource.sock_buf.sockfd = wt->evList[i].data.fd;
				task.taskResource.sock_buf.buf = new std::string();

				ioPool::addTask(task);
				
			}
		}
	}
}

//TODO 添加任务 Acceptrt -> netWorker
int WorkerThread::addTask(int fdTask)
{
	std::cout << "Net thread  " << this->no << " get new connect. fd = " << fdTask<<endl;
	LOGERR << "Net thread  " << this->no << " get new connect. fd = " << fdTask;
	epoll_event ev;
	ev.events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLET;
	ev.data.fd = fdTask;
	mClientNum.lock();
	ClientNum++;
	mClientNum.unlock();
	int res = epoll_ctl(fdEpoll, EPOLL_CTL_ADD, fdTask, &ev);
		cout << "Event list: {";
	for(int i=0; i < ClientNum; i++){
		std::cout << "fd: " << evList[i].data.fd << " ";
	}
	std::cout << "}" <<std::endl;
	return res;
}

//TODO 移除连接
int WorkerThread::removeConn(int fd)
{
	std::cout << "Fun WorkerThread::removeConn( " << fd << " )" <<std::endl;
	
	//查找
	epoll_event *p = nullptr;
	for(int i=0; i<ClientNum; i++){
		if(evList[i].data.fd == fd){
			p = &evList[i];
		}
	}
	if(p==nullptr){ 
		LOGERR << "Cannot found client";
		return -1;
	}

	//删除
	mClientNum.lock();
	ClientNum--;
	mClientNum.unlock();
	std::cout << "remove client " << fd;
	LOGINFO << "remove client " << fd;
	int res = epoll_ctl(fdEpoll, EPOLL_CTL_DEL, fd, p);
	::close(fd);
	cout << "Event list: {";
	for(int i=0; i < ClientNum; i++){
		std::cout << "fd: " << evList[i].data.fd << " ";
	}
	std::cout << "}" <<std::endl;
	return res;
}

//TODO 停止WorkerThread
void WorkerThread::stop()
{
	stopFlag = true;
}