#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <queue>
#include <vector>
#include <string.h>
#include "WorkThread.h"
#include "ThreadPool.h"
#include "EditDistance.h"

void WorkThread::relate_threadpool(ThreadPool *p_threadpool) {
	_p_threadpool = p_threadpool;
}

void WorkThread::run() {
	while (true) {
		_p_threadpool->get_task(_task);
		query(_task._keyword);
	}
}

struct word_edit_freq {
	std::string word;
	int edit_dis;
	std::size_t freq;
};

struct cmp {
	bool operator()(const word_edit_freq &a, const word_edit_freq &b) {
		if (a.edit_dis != b.edit_dis)
			return a.edit_dis > b.edit_dis;
		else
			return a.freq < b.freq;
	}
};

void send(std::priority_queue<word_edit_freq, std::vector<word_edit_freq>, cmp> &q, const Task &task) {
	//将查询结果发送出去
	char send_buf[1024];
	memset(send_buf, 0, 1024);
	std::string send_content;

	for (int i = 0; (i != 10) && !q.empty(); ++i) {
		send_content += q.top().word;
		send_content += "\n";
		q.pop();
	}

	strcpy(send_buf, send_content.c_str());
	sendto(task._server_fd, send_buf, sizeof(send_buf), 0,
			(struct sockaddr*) &task._client_addr, sizeof(task._client_addr));
}

void WorkThread::query(const std::string &word) {
	const std::map<std::string, std::size_t> word_dict =
			_p_threadpool->get_word_dict(); //取得词典
	std::map<std::string, std::size_t>::const_iterator iter;
	std::priority_queue<word_edit_freq, std::vector<word_edit_freq>, cmp> _q;
	//从词典中逐个计算单词与查询词的编辑距离，把每个单词、编辑距离、词频存入一个结构体
	//按自己的策略将符合条件的结构体放入优先级队列
	for (iter = word_dict.begin(); iter != word_dict.end(); ++iter) {
		word_edit_freq wef;
		wef.word = iter->first;
		EditDistance edit_dis(iter->first, word);
		wef.edit_dis = edit_dis.get_edit_dis();
		wef.freq = iter->second;
		if (wef.edit_dis < 3)
			_q.push(wef);
	}
	send(_q, _task);
}
