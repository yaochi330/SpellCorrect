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
	//std::cout << _server_addr.sin_port << " " << _server_addr.sin_addr.s_addr << std::endl;//debug
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
	recv_buf->_server_fd = _server_fd;
	recv_buf->_client_addr = _client_addr;
}

void UDPServer::sendTo(char *send_buf) {
	sendto(_server_fd, send_buf, send_size, 0,
			(struct sockaddr *) &_client_addr, sizeof(_client_addr));
}

//void UDPServer::set_send_addr(const struct sockaddr_in &client_addr) {
//	_client_addr = client_addr;
//}
//
//int UDPServer::get_fd() {
//	return _server_fd;
//}
