/*
 * ThreadPool.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: yaochi
 */
#include <iostream>
#include "ThreadPool.h"

ThreadPool::ThreadPool(std::vector<WorkThread>::size_type thread_num) :
		_is_start(false), _thread_num(thread_num), _thread_vec(_thread_num), _task_que(),
		_mutex(), _cond(_mutex) {
	for (std::vector<WorkThread>::iterator iter = _thread_vec.begin();
			iter != _thread_vec.end(); ++iter) {
		iter->relate_threadpool(this);
	}
	on();
}

ThreadPool::~ThreadPool() {
	off();
}

void ThreadPool::on() {
	if (_is_start == false) {
		_is_start = true;
		for (std::vector<WorkThread>::iterator iter = _thread_vec.begin();
				iter != _thread_vec.end(); ++iter) {
			iter->start();
		}
	}
}

void ThreadPool::off() {
	if (_is_start == true) {
		_is_start = false;
		_cond.notifyall();
		if (!_task_que.empty())
			_task_que.pop();
	}
}

bool ThreadPool::add_task(const Task &task) {
	MutexLockGuard _guard(_mutex);
	bool ret = false;
	if (_is_start) {
		_task_que.push(task);
		_cond.notifyall();
		ret = true;
	}
	return ret;
}

bool ThreadPool::get_task(Task &task) {
	MutexLockGuard _guard(_mutex);
	bool ret = false;
	while (_is_start && _task_que.empty()) {
		_cond.wait();
	}
	if (_is_start && !_task_que.empty()) {
		task = _task_que.front();
		_task_que.pop();
		ret = true;
	}
	return ret;
}
