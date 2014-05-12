#include <fstream>
#include <sstream>
#include <stdint.h>
#include "ThreadPool.h"

ThreadPool::ThreadPool(std::vector<WorkThread>::size_type thread_num) :
		_is_start(false), _thread_num(thread_num), _thread_vec(_thread_num), _task_que(), _mutex(), _cond(
				_mutex) {
	for (std::vector<WorkThread>::iterator iter = _thread_vec.begin();
			iter != _thread_vec.end(); ++iter) {
		iter->relate_threadpool(this); //调用每个工作线程的relate函数将线程池自身注册到每个工作线程中
	}
	_sync_cache_thread.relate_threadpool(this); //同上
	on();
}

ThreadPool::~ThreadPool() {
	off();
}

void divide(std::vector<std::string> &arr, const std::string &word) {
	char buf[3];
	for (std::string::const_iterator iter = word.begin(); iter != word.end();
			++iter) {
		if (*iter & 0x80) {
			buf[0] = *iter;
			buf[1] = *(++iter);
			buf[2] = '\0';
		} else {
			buf[0] = *iter;
			buf[1] = '\0';
		}
		arr.push_back(std::string(buf));
	}
}

void add_to_dict(
		std::unordered_map<std::string,
				std::unordered_map<std::string, std::size_t> > &word_dict,
		const std::string &word, const std::size_t freq) {
	std::vector<std::string> arr;
	divide(arr, word); //将字符串逐个拆分成汉字或字母
	//根据每个字建索引
	for (std::vector<std::string>::const_iterator iter = arr.begin();
			iter != arr.end(); ++iter) {
		(word_dict[*iter])[word] = freq;
	}
}

void ThreadPool::load_dict() {
	std::ifstream in_conf("../conf/dict.conf");
	std::string dict_name;
	while (in_conf >> dict_name) {
		std::ifstream in_dict(dict_name.c_str());
		std::string word;
		std::size_t freq;
		std::string line;
		while (getline(in_dict, line)) {
			std::istringstream is(line);
			is >> word >> freq;
			add_to_dict(_word_dict, word, freq);
		}
		in_dict.close();
		in_dict.clear();
	}
	in_conf.close();
	in_conf.clear();
}

void ThreadPool::on() {
	if (_is_start == false) {
		_is_start = true;
		for (std::vector<WorkThread>::iterator iter = _thread_vec.begin();
				iter != _thread_vec.end(); ++iter) {
			iter->start();
		}
		load_dict(); //载入词典
		_sync_cache_thread.start(); //开启同步cache线程
	}
}

void ThreadPool::off() {
	if (_is_start == true) {
		_is_start = false;
		_cond.notifyall(); //通知所有正在阻塞的工作线程
		if (!_task_que.empty()) //清空任务队列
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

const std::unordered_map<std::string,
		std::unordered_map<std::string, std::size_t> > &ThreadPool::get_word_dict() {
	return _word_dict;
}

const std::vector<WorkThread> &ThreadPool::get_thread_vector() const {
	return _thread_vec;
}
