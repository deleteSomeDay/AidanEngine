#include "TCPClient.h"
#include <iostream>
#include <thread>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "PacketHandle.h"

#pragma comment(lib,"ws2_32.lib")
#define BUF_SIZE 32768

TCPClient::TCPClient()
 : m_RecvBuffer(BUF_SIZE), m_Port(-1), m_Sock(-1)
{

}
TCPClient::~TCPClient()
{
	closesocket(m_Sock);
}

void TCPClient::networkThread()
{
	while (m_Connected)
	{
		char inBuf[4096] = { 0, };

		int n = ::recv(m_Sock, inBuf, 4096, 0);

		if (n < 1)
		{
			Sleep(0); ///< for cpu low-utilization
			continue;
		}

		
		if (!m_RecvBuffer.Write(inBuf, n))
		{ 
			assert(false);
		}

		processPacket();
	}
}
bool TCPClient::connect(std::string serverAddr, int port)
{

	if (m_Connected)
		return false;
	
	struct addrinfo *result,
		*ptr,
		hints;


	m_ServerAddr = serverAddr;
	m_Port = port;
	m_RecvBuffer.BufferReset();

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(m_ServerAddr.c_str(), std::to_string(m_Port).c_str(), &hints, &result) != 0)
	{
		std::cout << "getaddrinfo invalid" << std::endl;
		return false;
	}

	ptr = result;
	m_Sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (m_Sock == INVALID_SOCKET)
	{
		std::cout << "Socket Invalid" << std::endl;
		return false;
	}

	if(SOCKET_ERROR == ::connect(m_Sock, (sockaddr *) ptr->ai_addr, (int)ptr->ai_addrlen))
	{
		std::cout << WSAGetLastError() << std::endl;
		std::cout << "Connect Failed" << std::endl;
		return false;
	}

	int opt = 1;
	setsockopt(m_Sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int));

	m_Connected = true;
	/// receive thread start
	std::thread(std::bind(&TCPClient::networkThread,this)).detach();
		
	return true;
}

void TCPClient::disconnect()
{
	m_Connected = false;
	m_Playerid.clear();

	if (SOCKET_ERROR == ::shutdown(m_Sock, SD_BOTH))
	{
		std::cout << "SOCKET SHUTDOWN FAILED " << WSAGetLastError() << std::endl;
	}
}

void TCPClient::requestEndTurn()
{
	if (!m_Connected)
		return;

	EndTurnRequest sendData;

	send((const char*)&sendData, sizeof(EndTurnRequest));
	
}

void TCPClient::requestMatch()
{
	if (!m_Connected)
	{
		return;
	}

	MatchRequest sendData;

	//sprintf_s();
}

void TCPClient::requestAttack()
{
	
	
}
void TCPClient::requestDraw()
{

}
void TCPClient::requestGold()
{

}
void TCPClient::requestSurrender()
{

}

void TCPClient::processPacket()
{

	while (true)
	{
		PacketHeader header;

		if (false == m_RecvBuffer.Peek((char*)&header, sizeof(PacketHeader)))
			break;


		if (header.m_Size > m_RecvBuffer.GetStoredSize())
			break;


		switch (header.m_Type)
		{

		case PKT_ENDTURN:
		{
			
		}
		break;

		default:
			assert(false);
		}

	}
}



bool TCPClient::send(const char* data, int length)
{
	int count = 0;
	while (count < length)
	{
		int n = ::send(m_Sock, data + count, length, 0);
		if (n == SOCKET_ERROR)
		{
			std::cout << "SEND ERROR\n";
			return false;
		}
		count += n;
		length -= n;
	}

	return true;
}