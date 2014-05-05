#include <iostream>
#include <fstream>
#include "ThreadPool.h"
#include "UDPServer.h"
#include "Task.h"

void read_conf(short int &port, std::string &ip) {
	std::ifstream in("../conf/server.conf");
	in >> port >> ip;
	//std::cout << port << " " << ip << std::endl;//debug
	in.clear();
	in.close();
}

int main() {
	short int port;
	std::string ip;
	read_conf(port, ip); //从配置文件读ip和端口号

	ThreadPool threadpool;
	std::cout << "线程池开启" << std::endl;
	UDPServer server(port, ip);
	std::cout << "UDP服务开启" << std::endl;
	while (true) {
		Task task;
		server.recvFrom(&task);
		threadpool.add_task(task);
	}
}
