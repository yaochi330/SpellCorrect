#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <queue>
#include <vector>
#include <string.h>
#include "UDPServer.h"
#include "WorkThread.h"
#include "ThreadPool.h"
#include "EditDistance.h"
#include "EncodingConverter.h"
#include "Query.h"

void WorkThread::relate_threadpool(ThreadPool *p_threadpool) {
	_p_threadpool = p_threadpool;
}

void WorkThread::run() {
	while (true) {
		_p_threadpool->get_task(_task);
		std::cout << _task._keyword << std::endl; //往日志打印用户发送来的数据
		Query _query(_p_threadpool->get_word_dict(), _thread_cache);

		char send_buf[1024];
		memset(send_buf, 0, 1024);
		EncodingConverter trans;
		strcpy(send_buf, (trans.gbk_to_utf8(_query.search(trans.utf8_to_gbk(_task._keyword))) + "\n").c_str());
		sendto(_task._server_fd, send_buf, sizeof(send_buf), 0,
				(struct sockaddr*) &_task._client_addr,
				sizeof(_task._client_addr));
	}
}

Cache &WorkThread::getCache() const {
	return _thread_cache;
}
