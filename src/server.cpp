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
	/*重定向标准输出流*/
	std::ofstream out("../log/server.log", std::ofstream::out | std::ofstream::app);
	std::streambuf *filebuf = out.rdbuf(); //无参版本rdbuf函数用于获得输入输出缓冲区
	std::cout.rdbuf(filebuf); //带一个参数版本的rdbuf函数用于将一个流的缓冲区关联另一个流缓冲区，并清除错误状态标志

	/*从配置文件读ip和端口号*/
	short int port;
	std::string ip;
	read_conf(port, ip);

	ThreadPool threadpool;
	std::cout << "线程池开启" << std::endl;
	UDPServer udp_server(port, ip);
	std::cout << "UDP服务开启" << std::endl;
	while (true) {
		Task task;
		udp_server.recvFrom(&task);
		threadpool.add_task(task);
	}
}
