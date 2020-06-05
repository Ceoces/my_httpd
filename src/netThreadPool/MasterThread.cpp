#include "MasterThread.h"
#include "netThreadPoolInterface.h"


bool MasterThread::setAddr(int Port, std::string Address)
{
	//Init struct sockaddr_in Address
	memset(&this->ServerAddr, 0, sizeof(struct sockaddr_in));

	//String to char *
	int len = Address.length();
	char *addr = static_cast<char *>(malloc(sizeof(char) * 16));
	if (len < 7 || len>15) {
		throw std::string("The Address in config file is wrong.");
	}
	for (int i = 0; i < len; i++) {
		addr[i] = Address[i];
	}
	addr[len] = '\0';

	//Set address
	this->ServerAddr.sin_addr.s_addr = inet_addr(addr);
	if (this->ServerAddr.sin_addr.s_addr == INADDR_NONE) {
		throw std::string("The Address in config file is wrong.");
	}
	free(addr);

	this->ServerAddr.sin_family = AF_INET;


	if (Port>=65535) {
		this->ServerAddr.sin_port = static_cast<short unsigned int>(htonl(Port));
	}
	else {
		this->ServerAddr.sin_port = static_cast<unsigned short  int>(htons(Port));
	}
}

MasterThread::MasterThread(int Port, std::string Address)
{
	this->fdServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	this->setAddr(Port, Address);
}

void MasterThread::setMaxConn(int n)
{
	this->maxConn = n;
}

void MasterThread::bindPool(ThreadPool * p)
{
	this->threadPool = p;
}

int MasterThread::startListen()
{
	//Bind Address
	if (bind(this->fdServer, reinterpret_cast<struct sockaddr *>(&this->ServerAddr), sizeof(this->ServerAddr)) == -1)
	{
		throw errno;
		return 0;
	}

	//Listen Port
	if (listen(this->fdServer, 5) == -1) {
		throw errno;
		return 0;
	}
	return 1;
}

MasterThread::~MasterThread()
{
}

void MasterThread::run()
{
	int fdConn = -1;
	struct sockaddr_in addrConn;
	size_t len = sizeof(addrConn);

	int optval = 1;
	setsockopt(fdServer, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	setsockopt(fdServer, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof optval);

	while (true) {
		fdConn = accept(fdServer, reinterpret_cast<sockaddr *>(&addrConn), reinterpret_cast<socklen_t *>(&len));
		//push into thread_pool
		{
			conn_info conn;
			conn.addr = addrConn;
			conn.fd = fdConn;
			conn.thread_num = this->threadPool->addTask(fdConn);
			LOGINFO << "Give "<< inet_ntoa(addrConn.sin_addr)<<'/'<<htons(addrConn.sin_port) <<" to " << conn.thread_num << "thread.";
			std::unique_lock<std::mutex> lock(m_conn_list);
			conn_list.push_back(conn);
			lock.unlock();
		}
	}
}
