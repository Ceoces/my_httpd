#include "./ioPool.h"

int ioPool::_size = -1;
std::vector<ioWorker *> ioPool::workerList;

//TODO: 添加任务
void ioPool::addTask(ioTask task)
{
    cout << "ioPool:addTask({fd:"<< task.taskResource.sock_fd.sockfd <<"})" << endl;
    std::unique_lock<std::mutex> lock(ioPoolmcv);

    ioTaskQueue.push(task);
    
    lock.unlock();
    ioPoolcv.notify_all();
    
}

//TODO 启动io线程池
bool ioPool::start(int size = IO_THREAD_DEFAULT_SIZE)
{
    if(_size>0) return false;
    _size = size;
    workerList.clear();
    for(int i=0; i<_size; i++){
        workerList.push_back(new ioWorker(i));
        workerList[i]->start();
    }
    return true;
}


//TODO 关闭io线程池
void ioPool::stop()
{
    for(int i=0; i<workerList.size(); i++){
        workerList[i]->stop();
    }
}