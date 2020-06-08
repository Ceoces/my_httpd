#include "ThreadPool.h"


std::vector<conn_info> conn_list;
std::mutex m_conn_list;
netThreadPoolInterface *pNetPoolInterface = nullptr;

ThreadPool::ThreadPool(int size) : init_size(size)
{

}

ThreadPool::~ThreadPool()
{

}

void ThreadPool::start()
{
	for (int i = 0; i < this->init_size; i++) {
		this->workerList.push_back(new WorkerThread(i + 1));
		thread t(do_work, workerList[i]);
		t.detach();
	}
}

//TODO 添加连接
bool ThreadPool::addTask(int fdTask)
{
	int no = 0;
	for (int i = 0; i < this->workerList.size(); i++) {
		if (workerList[i]->ClientNum < workerList[no]->ClientNum) {
			no = i;
		}
	}
	workerList[no]->addTask(fdTask);
	return no;
}

//TODO 移除连接
bool ThreadPool::delConn(int fd)
{
	std::cout << "ThreadPool::delConn( " << fd << " )" <<std::endl;
	std::unique_lock<std::mutex> lock(m_conn_list);

	vector<conn_info>::iterator it;
	bool flag = false;
	cout << "conn_info: ";
	for(auto &it : conn_list){
		cout << "{ fd:" << it.fd << ", t_no:" << it.thread_num << " } ";
	}
	cout <<endl;
	for(it = conn_list.begin(); it != conn_list.end(); it++){
		if(it->fd == fd){
			
			flag = true;
			break;
		}
	}
	int del_thread_no = it->thread_num;
	conn_list.erase(it);
	lock.unlock();
	workerList[del_thread_no]->removeConn(fd);
	if(!flag){
		LOGERR << "Cannot find the client which need to delete";
		std::cout << "Cannot find the client("<< fd <<") which need to delete" << std::endl;
	}
	return flag;
}

//TODO 停止线程池
void ThreadPool::stop(){
	for(int i=0;i<init_size; i++){
		workerList[i]->stop();
	}
}