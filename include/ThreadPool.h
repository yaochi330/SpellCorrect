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
#include <unordered_map>
#include <string>
#include "WorkThread.h"
#include "Task.h"
#include "SyncCacheThread.h"

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

	const std::unordered_map<std::string, std::unordered_map<std::string, std::size_t> > &get_word_dict(); //工作线程查询时用来获得词典的引用
	/*SyncCacheThread的接口*/
	const std::vector<WorkThread> &get_thread_vector() const; //获得各工作线程，以便同步线程获得各工作线程的cache进行更新工作
private:
	bool _is_start;
	std::vector<WorkThread>::size_type _thread_num; //线程数量
	std::vector<WorkThread> _thread_vec; //存放各工作线程
	std::queue<Task> _task_que; //任务队列，存放用户发送来的任务
	std::unordered_map<std::string, std::unordered_map<std::string, std::size_t> > _word_dict; //词典
	void load_dict(); //线程池开启时从磁盘读入词典
	SyncCacheThread _sync_cache_thread; //同步磁盘cache和各线程cache的类
	mutable MutexLock _mutex;
	mutable Condition _cond;
};

#endif /* THREADPOOL_H_ */
