#ifndef WORKTHREAD_H_
#define WORKTHREAD_H_
#include <string>
#include "Thread.h"
#include "Task.h"

class ThreadPool;

class WorkThread : public Thread{
public:
	void relate_threadpool(ThreadPool*);
private:
	ThreadPool *_p_threadpool;
	Task _task;
	void run();
	void query(const std::string &);
};
#endif
