#ifndef THREAD_H_
#define THREAD_H_
#include <pthread.h>

class Thread{
public:
	Thread();
	virtual ~Thread();
	void start();
private:
	pthread_t _tid; //线程id
	pthread_attr_t _attr; //线程属性，JOINABLE或DETACHED
	bool _is_on; //线程是否开启标志
	static void *thread_func(void *arg);
	virtual void run() = 0;
};
#endif
