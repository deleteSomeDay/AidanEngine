#pragma once
#include "Stdafx.h"
class TCPServer
{
private:
	fd_set m_Master;    // master file descriptor list
	fd_set m_ReadFDs;  // temp file descriptor list for select()
	int m_FDmax;        // maximum file descriptor number

	int m_Listener;

	struct addrinfo m_Hints, *m_Ptr, *m_Result;

	 const char* m_Port;
public:
	TCPServer();
	~TCPServer();

	bool init(const char* port);

	void acceptLoop();

	bool send(char* data, int msglen, int fd);

private:
	void *get_in_addr(struct sockaddr *sa)
	{
		if (sa->sa_family == AF_INET)
			return &(((struct sockaddr_in*)sa)->sin_addr);
		return &(((struct sockaddr_in6*)sa)->sin6_addr);
	}
};