#ifndef WORKTHREAD_H_
#define WORKTHREAD_H_
#include "Thread.h"
class ThreadPool;
class WorkThread : public Thread {
public:
	void relate_threadpool(ThreadPool*);
private:
	ThreadPool *_p_threadpool;
	void run();
};
#endif
