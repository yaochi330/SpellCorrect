MAIN:=./bin/server
OBJS:= ./src/server.cpp ./src/Thread.cpp ./src/ThreadPool.cpp ./src/UDPServer.cpp ./src/WorkThread.cpp ./include/Condition.h ./include/MutexLock.h ./src/EncodingConverter.cpp 
$(MAIN):$(OBJS)
	g++ -o $@ $(OBJS) -lpthread -I./include