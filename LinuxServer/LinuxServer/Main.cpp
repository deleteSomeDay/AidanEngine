#include "Stdafx.h"
#include "TCPServer.h"
#define PORT "5999"
#define BACKLOG 10


int main(void)
{
	std::cout << "Start" << std::endl;
	TCPServer tcpServer = TCPServer();

	tcpServer.init(PORT);

	std::cout << "Server Running" << std::endl;

	tcpServer.acceptLoop();

	std::cout << "server done" << std::endl;
	
	return 0;
}