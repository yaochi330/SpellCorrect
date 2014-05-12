/*
 * SyncCacheThread.h
 *
 *  Created on: May 10, 2014
 *      Author: yaochi
 */

#ifndef SYNCCACHETHREAD_H_
#define SYNCCACHETHREAD_H_
#include <unordered_map>
#include "Thread.h"
#include "Cache.h"

class ThreadPool;

class WorkThread;

class SyncCacheThread : public Thread{
public:
	void relate_threadpool(ThreadPool *);
private:
	ThreadPool *_p_threadpool;
	std::unordered_map<std::string, std::string>  _all_cache;
	void run();
	void load_disk_cache(); //载入磁盘cache
	void update_to_thread_cache(); //将磁盘cache更新到各线程的cache
	void sync_cache();//同步总cache与各线程cache
	void write_disk_cache() const; //写回磁盘cache
};

#endif /* SYNCCACHETHREAD_H_ */
