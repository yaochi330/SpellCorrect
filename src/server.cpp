#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
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

void Daemon() {
	const int MAXFD = 64;
	if (0 != fork())
		exit(0);
	setsid();
	umask(0);
	for(int i = 0;i<MAXFD;i++) //尽可能关闭所有从父进程继承来的文件
			  close(i);
}

int main() {
	Daemon();
	//重定向标准输出流
	std::ofstream out("../log/server.log", std::ofstream::out | std::ofstream::app);
	std::streambuf *filebuf = out.rdbuf();
	std::cout.rdbuf(filebuf);

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
