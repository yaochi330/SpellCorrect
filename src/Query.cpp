/*
 * Query.cpp
 *
 *  Created on: May 9, 2014
 *      Author: yaochi
 */
#include "Query.h"
#include "EditDistance.h"
#include <iostream>
#include <set>

#define RETNUM 1

Query::Query(
		const std::unordered_map<std::string,
				std::unordered_map<std::string, std::size_t> > &word_dict, Cache &cache) :
		_word_dict(word_dict), _cache(cache) {

}

void Query::divide(std::vector<std::string> &arr, const std::string &word) {
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
		arr.push_back(buf);
	}
}

std::string Query::search(const std::string &word){
	if(_cache.is_exist(word)) {
		//std::cout << "exist" << std::endl; //debug
		return _cache.get_content(word);
	}
	else {
		//std::cout << "not exist" << std::endl; //debug
		std::string content = search_dict(word);
		if (content != "<null>") //如果没有匹配结果，则不加入cache
			_cache.put_content(word, content);
		return content;
	}
}

std::string Query::search_dict(const std::string &word) {
	std::set<word_edit_freq> _set; //去除重复词
	std::vector<std::string> arr;
	divide(arr, word);
	for (auto &ix : arr) {
		std::unordered_map<std::string,
				std::unordered_map<std::string, std::size_t> >::const_iterator iter;
		iter = _word_dict.find(ix); //查看用户输入的汉子或字符再词典里是否存在
		if (iter != _word_dict.end()) {
			for (auto &entry : _word_dict.at(ix)) {
				EditDistance edit_dis(entry.first, word);
				word_edit_freq wef;
				wef.word = entry.first;
				wef.edit_dis = edit_dis.get_edit_dis();
				wef.freq = entry.second;
				if (wef.edit_dis < 4) {
					_set.insert(wef);
					//_q.push(wef);
				}
			}
		}
	}

	for(auto &ix : _set) { //将已去重的词放入优先级队列
		_q.push(ix);
	}

	std::string content;
	if(_q.empty()) {
		content += "<null>";
	}
	for (int i = 0; i != RETNUM && !_q.empty(); ++i) {
		content += _q.top().word;
		_q.pop();
	}
	return content;
}
