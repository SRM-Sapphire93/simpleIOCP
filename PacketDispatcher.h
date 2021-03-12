#pragma once
#define PACKET_FUNCTION_DECLARE(type) void ON_##type(char* _streamStart)
class CPacketDispatcher
{

public :
	static void ProcessPacket(char * _streamStart);
};
PACKET_FUNCTION_DECLARE(CLIENT_TO_SERVER_MESSAGE);
