#pragma once
enum PacketTypes
{
	PKT_NONE = 0,

	PKT_ENDTURN = 10,



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