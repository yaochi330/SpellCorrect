#ifndef TASK_H_
#define TASK_H_
#include <netinet/in.h>
struct Task {
	char _keyword[1024];
	int _server_fd;
	struct sockaddr_in _client_addr;
};

#endif /* TASK_H_ */
