/*
 * Condition.h
 *
 *  Created on: 2014年4月22日
 *      Author: yaochi
 */

#ifndef CONDITION_H_
#define CONDITION_H_
#include "MutexLock.h"
#include "noncopyable.h"

class Condition : public noncopyable{
public:
	Condition(MutexLock &mutex) : _mutex(mutex) {
		pthread_cond_init(&_cond, NULL);
	}
	~Condition() {
		pthread_cond_destroy(&_cond);
	}
	void wait() {
		pthread_cond_wait(&_cond, _mutex.get_mutex_pointer());
	}
	void notify() {
		pthread_cond_signal(&_cond);
	}
	void notifyall() {
		pthread_cond_broadcast(&_cond);
	}
private:
	pthread_cond_t _cond;
	MutexLock &_mutex;
};

#endif /* CONDITION_H_ */
