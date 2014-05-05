/*
 * MutexLock.h
 *
 *  Created on: 2014年4月22日
 *      Author: yaochi
 */

#ifndef MUTEXLOCK_H_
#define MUTEXLOCK_H_
#include <pthread.h>
#include "noncopyable.h"

class MutexLock : public noncopyable {
public:
	MutexLock() {
		pthread_mutex_init(&_mutex, NULL);
	}

	~MutexLock() {
		pthread_mutex_destroy(&_mutex);
	}

	void lock() {
		pthread_mutex_lock(&_mutex);
	}
	void unlock(){
		pthread_mutex_unlock(&_mutex);
	}
	pthread_mutex_t *get_mutex_pointer() {
		return &_mutex;
	}
private:
	pthread_mutex_t _mutex;
};

class MutexLockGuard : public noncopyable{
public:
	MutexLockGuard(MutexLock &mutex) :_mutex(mutex) {
		_mutex.lock();
	}
	~MutexLockGuard(){
		_mutex.unlock();
	}
private:
	MutexLock &_mutex;
};

#endif /* MUTEXLOCK_H_ */
