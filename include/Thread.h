#ifndef THREAD_H_
#define THREAD_H_
#include <pthread.h>

class Thread{
public:
	Thread();
	virtual ~Thread();
	void start();
private:
	pthread_t _tid;
	pthread_attr_t _attr;
	bool _is_on;
	static void *thread_func(void *arg);
	virtual void run() = 0;
};
#endif
