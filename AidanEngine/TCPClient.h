#pragma once
#include <string>
#include <winsock2.h>
#include "CircularBuffer.h"
class TCPClient
{
private:
	std::string m_ServerAddr;
	std::string m_Playerid;
	SOCKET m_Sock;
	int m_Port;
	CircularBuffer m_RecvBuffer;
	bool m_Connected;
public:

	TCPClient();
	~TCPClient();

	bool connect(std::string serverAddr, int port);
	
	void disconnect();

	void networkThread();

	void requestMatch();

	void requestEndTurn();
	void requestAttack();
	void requestDraw();
	void requestGold();
	void requestSurrender();
private:
	void processPacket();

	bool send(const char* data, int length);
};