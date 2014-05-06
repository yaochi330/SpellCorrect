/*
 * ThreadPool.h
 *
 *  Created on: Apr 28, 2014
 *      Author: yaochi
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include <vector>
#include <queue>
#include <map>
#include <string>
#include "WorkThread.h"
#include "Task.h"

#include "MutexLock.h"
#include "Condition.h"

#include "noncopyable.h"

class ThreadPool : public noncopyable{
public:
	ThreadPool(std::vector<WorkThread>::size_type thread_num = 4);
	~ThreadPool();

	void on();
	void off();
	bool add_task(const Task &task);
	bool get_task(Task &task);
	const std::map<std::string, std::size_t> &get_word_dict();
private:
	bool _is_start;
	std::vector<WorkThread>::size_type _thread_num;
	std::vector<WorkThread> _thread_vec;
	std::queue<Task> _task_que;
	std::map<std::string, std::size_t> _word_dict;
	mutable MutexLock _mutex;
	mutable Condition _cond;
};

#endif /* THREADPOOL_H_ */
