/*
 * SyncCacheThread.cpp
 *
 *  Created on: May 10, 2014
 *      Author: yaochi
 */

#include "SyncCacheThread.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <unistd.h>
#include "ThreadPool.h"
#include "WorkThread.h"

void SyncCacheThread::relate_threadpool(ThreadPool *p_threadpool) {
	_p_threadpool = p_threadpool;
}

void SyncCacheThread::run() {
	load_disk_cache();
	update_to_thread_cache(); //程序启动时将磁盘中的cache放到到各线程中
	sync_cache(); //同步磁盘cache与内存cache
}

void SyncCacheThread::load_disk_cache() {
	std::ifstream in("../conf/cache.conf");
	std::string disk_cache_file;
	in >> disk_cache_file;
	in.close();
	in.clear();
	in.open(disk_cache_file.c_str());
	std::string word; //查询词
	std::string content; //查询结果
	while (in >> word >> content) {
		_all_cache.insert(std::unordered_map<std::string, std::string>::value_type(word, content));
		//std::cout << word << " " << content << std::endl; //debug
	}
	in.close();
	in.clear();
}

void SyncCacheThread::update_to_thread_cache() {
	//将总cache载入到各线程cache中
	const std::vector<WorkThread> &thread_vec =
			_p_threadpool->get_thread_vector();
	for (std::vector<WorkThread>::const_iterator iter = thread_vec.begin();
			iter != thread_vec.end(); ++iter) {
		Cache &thread_cache = const_cast<Cache &>(iter->getCache());
		thread_cache.update_cache(_all_cache);
	}
}

void SyncCacheThread::sync_cache() {
	while (true) {
		sleep(30);
		const std::vector<WorkThread> &thread_vec =
				_p_threadpool->get_thread_vector();
		//合并各线程中的cache
		for (std::vector<WorkThread>::const_iterator iter = thread_vec.begin();
				iter != thread_vec.end(); ++iter) {
			const std::unordered_map<std::string, std::string> &thread_cache =
					(iter->getCache()).get_cache();
			for (auto &ix : thread_cache) {
				_all_cache.insert(ix);
			}
		}
		update_to_thread_cache();
		write_disk_cache();
	}
}

void SyncCacheThread::write_disk_cache() const {
	std::ifstream in("../conf/cache.conf");
	std::string disk_cache_file;
	in >> disk_cache_file;
	in.close();
	in.clear();
	std::ofstream out(disk_cache_file.c_str());
	std::string word; //查询词
	std::string content; //查询结果
	for (auto &ix : _all_cache) {
		out << ix.first << "\t" << ix.second << std::endl;
	}
}
