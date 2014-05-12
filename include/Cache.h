/*
 * Cache.h
 *
 *  Created on: May 10, 2014
 *      Author: yaochi
 */

#ifndef CACHE_H_
#define CACHE_H_

#include <unordered_map>
#include <string>
#include "MutexLock.h"

class Cache {
public:
	//查找操作接口
	bool is_exist(const std::string &) const;
	const std::string &get_content(const std::string &) const;
	void put_content(const std::string &, const std::string &);
	//同步操作接口
	const std::unordered_map<std::string, std::string> &get_cache() const;
	void update_cache(const std::unordered_map<std::string, std::string> &);
private:
	std::unordered_map<std::string, std::string> _cache; //每个线程的cache
	mutable MutexLock _mem_to_disk_lock; //存放新的查询内容与同步线程取map的互斥锁
	mutable MutexLock _disk_to_mem_lock; //查询时取内容与同步线程更新map的互斥锁
};

#endif /* CACHE_H_ */
