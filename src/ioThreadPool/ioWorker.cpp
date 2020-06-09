#include "./ioWorker.h"

queue<ioTask> ioTaskQueue;
std::mutex ioPoolmcv;
std::condition_variable ioPoolcv;

ioWorker::ioWorker(int no) : _no(no)
{
}

ioWorker::~ioWorker()
{
}

void ioWorker::working()
{
    //TODO IO thread
    std::cout << "IO worker " << _no << " start." << std::endl;
    LOGINFO << "IO worker " << _no << " start.";
    while (!_stop)
    {
        std::unique_lock<std::mutex> lock(ioPoolmcv);
        while (ioTaskQueue.empty())
        {
            ioPoolcv.wait(lock);
        }
        ioTask task = ioTaskQueue.front();
        ioTaskQueue.pop();
        lock.unlock();

        LOGINFO << "IO worker " << _no << " get data from " << task.taskResource.sock_fd.sockfd;

        do_task(task);
    }
    LOGINFO << "ioWorker " << this->_no << " stop.";
}

//TODO 启动线程
void ioWorker::start()
{
    _worker = new std::thread(&ioWorker::working, this);
    std::cout << "starting io thread" <<std::endl;
    _worker->detach();
}

//TODO 处理事件
void ioWorker::do_task(ioTask task)
{
    char buf[1025];
    switch(task.type){
        case ioTask::READ_FROM_SOCK:
        {
             int len = read(task.taskResource.sock_buf.sockfd, buf, 1024);
             if (len > 0) {
					buf[len] = '\0';
                    Request *req = new Request(buf, len);
                    LOGINFO << "ioWorker: get http request:" << req->getUrl();
                    string file_type = req->getUrl().substr(req->getUrl().find_last_of('.')+1);
                    req->do_request(task.taskResource.sock_buf.sockfd);
			}
			else if (len == 0) {
                if(!pNetPoolInterface->delConn(task.taskResource.sock_buf.sockfd)){
                   LOGERR << "Del conn field " << task.taskResource.sock_buf.sockfd;
                }
			}
			else{
                LOGERR << "IO Thread " << _no << " read error. Error info: " << strerror(errno);
			}
             break;
        }
        case  ioTask::FILE_TO_SOCK:
             break;
        case ioTask::WRITE_TO_SOCK:
             break;
    }
}

//TODO 关闭ioWorker
void ioWorker::stop()
{
    _stop = true;
}