/*
 * Cache.cpp
 *
 *  Created on: May 10, 2014
 *      Author: yaochi
 */

#include "Cache.h"

bool Cache::is_exist(const std::string &word) const {
	if(_cache.find(word) != _cache.end())
		return true;
	else
		return false;
}

const std::string &Cache::get_content(const std::string &word) const {
	MutexLockGuard _guard(_disk_to_mem_lock);
	return _cache.at(word);
}

void Cache::put_content(const std::string &word, const std::string &content) {
	MutexLockGuard _guard(_mem_to_disk_lock);
	_cache[word] = content;
}

const std::unordered_map<std::string, std::string> &Cache::get_cache() const {
	MutexLockGuard _guard(_mem_to_disk_lock);
	return _cache;
}

void Cache::update_cache(const std::unordered_map<std::string, std::string> &all_cache) {
	MutexLockGuard _guard(_disk_to_mem_lock);
	_cache = all_cache;
}
