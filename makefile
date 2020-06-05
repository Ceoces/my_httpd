srcConfig = src/config
srcThreadPool = src/netThreadPool
srcUtil = src/util
srcHttp = src/HttpModule
srclog = src/log
srcio = src/ioThreadPool
cc = g++ -w -g -std=c++1z

pthread = -lpthread

objects = main.o $(srcThreadPool)/MasterThread.o $(srcThreadPool)/WorkerThread.o $(srcThreadPool)/ThreadPool.o \
	 $(srcUtil)/util.o  $(srcConfig)/myConfig.o  $(srcHttp)/request.o $(srcHttp)/response.o $(srclog)/logger.o	\
	 $(srcio)/ioPool.o $(srcio)/ioWorker.o	

main :  $(srcUtil)/util.o  $(srcConfig)/myConfig.o  $(srcHttp)/request.o $(srcHttp)/response.o \
	  $(srcThreadPool)/MasterThread.o $(srcThreadPool)/WorkerThread.o $(srcThreadPool)/ThreadPool.o \
	  $(srclog)/logger.o  $(srcio)/ioPool.o $(srcio)/ioWorker.o 

	$(cc)  src/main.cpp   $^  $(pthread) -o main.out
	mv main.out bin/myhttpd

$(srcThreadPool)/MasterThread.o :  $(srclog)/logger.o
	$(cc) -c $(srcThreadPool)/MasterThread.cpp $^  -o $@

$(srcThreadPool)/WorkerThread.o : $(srcHttp)/request.o $(srclog)/logger.o
	$(cc) -c $(srcThreadPool)/WorkerThread.cpp  $^   -o $@

$(srcThreadPool)/ThreadPool.o :  $(srclog)/logger.o
	$(cc) -c $(srcThreadPool)/ThreadPool.cpp $^   $(pthread) -o $@

$(srcUtil)/util.o :  $(srclog)/logger.o
	$(cc) -c $(srcUtil)/util.cpp $^  -o $@

$(srcConfig)/myConfig.o : 
	$(cc)  -c $(srcConfig)/myConfig.cpp $^  -o $@

$(srcHttp)/request.o: $(srclog)/logger.o
	$(cc)  -c $(srcHttp)/request.cpp $^  -o $@

$(srcHttp)/response.o : $(srclog)/logger.o $(srcUtil)/util.o
	$(cc)  -c $(srcHttp)/response.cpp $^  -o $@

$(srclog)/logger.o : 
	$(cc) -c $(srclog)/logger.cpp $^  -o $@

$(srcio)/ioPool.o : 
	$(cc) -c $(srcio)/ioPool.cpp $^  -o $@

$(srcio)/ioWorker.o : $(srclog)/logger.o $(srcHttp)/response.o
	$(cc) -c $(srcio)/ioWorker.cpp $^  -o $@


clean : 
	rm -f  $(objects)
