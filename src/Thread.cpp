#include "Thread.h"
#include <stdexcept>

Thread::Thread() : _tid(0), _is_on(false) {
	pthread_attr_init(&_attr);
}

Thread::~Thread() {
	pthread_attr_destroy(&_attr);
}

void Thread::start() {
	if(!_is_on) {
		_is_on = true;
		if(pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_DETACHED)) {
			throw std::runtime_error("attr");
		}
		if(pthread_create(&_tid, &_attr, thread_func, this)) {
			throw std::runtime_error("thread create");
		}
	}
}

void *Thread::thread_func(void *arg) {
	Thread *p_thread = static_cast<Thread *>(arg);
	p_thread->run();
	return NULL;
}
