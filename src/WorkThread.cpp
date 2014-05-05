#include "WorkThread.h"
#include "ThreadPool.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>

void WorkThread::relate_threadpool(ThreadPool *p_threadpool) {
	_p_threadpool = p_threadpool;
}

void WorkThread::run() {
	while (true) {
		Task task;
		_p_threadpool->get_task(task);
		char send_buf[1024];
		memset(send_buf, 0, 1024);
		strcpy(send_buf, task._keyword);
		sendto(task._server_fd, send_buf, sizeof(send_buf), 0,
				(struct sockaddr*) &task._client_addr,
				sizeof(task._client_addr));
	}
}
