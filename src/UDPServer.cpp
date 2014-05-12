/*
 * Udp.cpp
 *
 *  Created on: 2014年4月14日
 *      Author: yaochi
 */

#include "UDPServer.h"
#include <iostream>
#include <stdexcept>

UDPServer::UDPServer(short int &port, const std::string &addr) :
		recv_size(1024), send_size(1024) {
	_server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (_server_fd == -1)
		throw std::runtime_error("create socket fail");
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(port);
	_server_addr.sin_addr.s_addr = inet_addr(addr.c_str());
	if (-1 == bind(_server_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)))
		throw std::runtime_error("bind socket fail");
}

UDPServer::~UDPServer() {
	close(_server_fd);
}

void UDPServer::recvFrom(Task *recv_buf) {
	socklen_t len = sizeof(_client_addr);
	recvfrom(_server_fd, recv_buf->_keyword, recv_size, 0,
			(struct sockaddr *) &_client_addr, &len);
	recv_buf->_server_fd = _server_fd; //将套接字描述符记录下来，给用户返回消息时使用
	recv_buf->_client_addr = _client_addr; //记录用户的地址信息
}

/*未使用*/
void UDPServer::sendTo(char *send_buf) {
	sendto(_server_fd, send_buf, send_size, 0,
			(struct sockaddr *) &_client_addr, sizeof(_client_addr));
}
