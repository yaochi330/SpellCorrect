/*
 * Udp.h
 *
 *  Created on: 2014年4月14日
 *      Author: yaochi
 */

#ifndef UDP_H_
#define UDP_H_
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstddef>
#include <stdlib.h>
#include "Task.h"

class UDPServer {
public:
	UDPServer(short int &, const std::string &);
	~UDPServer();
	void recvFrom(Task *);
	void sendTo(char *);
private:
	int _server_fd;
	struct sockaddr_in _server_addr, _client_addr;
	const std::size_t recv_size;
	const std::size_t send_size;
};

#endif /* UDP_H_ */
