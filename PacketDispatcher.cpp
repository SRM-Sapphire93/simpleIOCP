#include "PacketDispatcher.h"
#include "Net.h"
#include "PacketFunctions.h"
#define CASE_PACKET_FUNCTION(type,param) case type : ON_##type(param); break; 

void CPacketDispatcher::ProcessPacket(char * _streamStart)
{
	switch (_streamStart[0])
	{
		CASE_PACKET_FUNCTION(CLIENT_TO_SERVER_MESSAGE,_streamStart);

	default:
		break;
	}

}
PACKET_FUNCTION_DECLARE(CLIENT_TO_SERVER_MESSAGE)
{
	

	return;
}