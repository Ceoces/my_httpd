#include  <cstdio>
#include <iostream>
#include <string>
#include <cstdlib>

#include<signal.h>

using namespace std;

#include "config/myConfig.h"

#include "ioThreadPool/ioPool.h"
#include "netThreadPool/MasterThread.h"


#include "util/util.h"
#include "log/logger.h"

MasterThread *tMaster;

//TODO httpd入口
int main()
{
	signal(SIGINT, [](int signum)->void{
			cout << "SIGINT" <<endl;
			LOGINFO << "Get signal SIGINT";
			cout << "httpd will stop after 3 s" <<endl;
			ioPool::stop();
			tMaster->stop();
			LOGSTOP();
			alarm(3);
	});
	try {
		myConfig conf = Config::get();
		//打开log文件
		LOGSTART("myhttpd.log");

		LOGINFO << "Httpd bind address : " <<  conf["Address"] << "/"<< conf["Port"];
		LOGINFO << "Start io pool.";
		ioPool::start(2);

		tMaster = new MasterThread(atoi(conf["Port"].c_str()), conf["Address"]);
		tMaster->startListen();
		LOGINFO << "Start listen.";
		
		LOGINFO << "Start net pool.";
		pNetPoolInterface = new ThreadPool(1);
		pNetPoolInterface->start();

		tMaster->bindPool((ThreadPool *)pNetPoolInterface);



		LOGINFO << "Start master thread.";
		tMaster->run();
	}
	catch (string e) {
		cerr << e << endl;
		LOGERR << "Net thread pool start error : " << e ;
		exit(1);
	}
	catch (int e) {
		cerr << strerror(e) << endl;
		LOGERR << "Net thread pool start error : " << strerror(e);
	}
	
	
	getc(stdin);
    return 0;
}