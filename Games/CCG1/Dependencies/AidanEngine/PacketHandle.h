#pragma once

#define MAX_STRING_LEN 64
#define MAX_SESSION_LEN 128

enum PacketTypes
{
	PKT_NONE = 0,

	PKT_ENDTURN = 10,

	PKT_MATCH_REQUEST = 101,
	PKT_MATCH_WAIT = 102, //Wait before match starts
	PKT_MATCH_RESULT = 103,

	PKT_MAX = 1024
};

struct PacketHeader
{
	PacketHeader() : m_Size(0), m_Type(PKT_NONE) {}
	short m_Size;
	short m_Type; 
};

struct EndTurnRequest : PacketHeader
{
	EndTurnRequest()
	{
		m_Size = sizeof(EndTurnRequest);
		m_Type = PKT_ENDTURN;
	}
	
};

struct MatchRequest : PacketHeader
{
	MatchRequest()
	{
		m_Size = sizeof(MatchRequest);
		m_Type = PKT_MATCH_REQUEST;

		memset(m_PlayerName, 0, sizeof(m_PlayerName));
		memset(m_PlayerPass, 0, sizeof(m_PlayerPass));
	}

	char m_PlayerName[MAX_STRING_LEN];
	char m_PlayerPass[MAX_STRING_LEN];

};

//Waiting for Match Packet (Lobby of sorts)
struct MatchWait : PacketHeader
{
	MatchWait()
	{
		m_Size = sizeof(MatchWait);
		m_Type = PKT_MATCH_WAIT;

		bool m_OK = false;
	}
	bool m_OK;
};

struct MatchResult : PacketHeader
{
	MatchResult()
	{
		m_Size = sizeof(MatchResult);
		m_Type = PKT_MATCH_RESULT;

		m_Port = 0;
		memset(m_PlayerId, 0, sizeof(m_PlayerId));
		memset(m_IpAddress, 0, sizeof(m_IpAddress));
	}

	char m_PlayerId[MAX_SESSION_LEN];
	char m_IpAddress[MAX_STRING_LEN];
	int m_Port;

};