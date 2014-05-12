#ifndef WORKTHREAD_H_
#define WORKTHREAD_H_
#include <string>
#include "Thread.h"
#include "Task.h"
#include "Cache.h"

class ThreadPool;

class WorkThread : public Thread{
public:
	void relate_threadpool(ThreadPool*);
	//SyncCacheThread的接口
	Cache &getCache() const; //该返回类型不能为const，因为SyncCacheThread调用该方法就是企图调用Cache类的非const方法更新Cache
private:
	ThreadPool *_p_threadpool;
	Task _task;
	mutable Cache _thread_cache;
	void run();
	void query(const std::string &);
};
#endif
