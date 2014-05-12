/*
 * Query.h
 *
 *  Created on: May 9, 2014
 *      Author: yaochi
 */

#ifndef QUERY_H_
#define QUERY_H_

#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include "Cache.h"

struct word_edit_freq {
	std::string word;
	int edit_dis;
	std::size_t freq;
	inline bool operator<(const struct word_edit_freq &rhs) const {//去重时使用, 此处的函数const必须加上，否则编译会报错
		if (word < rhs.word)
			return true;
		else
			return false;
	}
};

struct cmp {
	bool operator()(const word_edit_freq &a, const word_edit_freq &b) {
		if (a.edit_dis != b.edit_dis)
			return a.edit_dis > b.edit_dis;
		else
			return a.freq < b.freq;
	}
};

class Query {
public:
	Query(const std::unordered_map<std::string, std::unordered_map<std::string, std::size_t> > &, Cache &);
	std::string search(const std::string &);
	std::string search_dict(const std::string &);
private:
	const std::unordered_map<std::string, std::unordered_map<std::string, std::size_t> > &_word_dict;
	Cache &_cache;
	std::priority_queue<word_edit_freq, std::vector<word_edit_freq>, cmp> _q; //存放查询结果的优先级队列
	void divide(std::vector<std::string> &arr, const std::string &word);

};

#endif /* QUERY_H_ */
