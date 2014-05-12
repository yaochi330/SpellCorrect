#ifndef TASK_H_
#define TASK_H_
#include <netinet/in.h>
struct Task {
	char _keyword[1024]; //用户的查询词
	int _server_fd; //服务器UDP套接字描述符
	struct sockaddr_in _client_addr; //客户端地址信息
};

#endif /* TASK_H_ */
