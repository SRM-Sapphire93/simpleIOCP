#pragma once
#include "Net.h"
#include "Stream.h"
#include "MPSCQueue.h"
#define PACKET_FUNCTION_DECLARE(type) void ON_##type(CStream* _packet, CSession* _session, packettype_t _type)
#define CASE_PACKET_FUNCTION(packet,session,type) case type : ON_##type(packet,session,type); break; 


class CSession;
class CPacketDispatcher
{
private :
	static CPacketDispatcher* m_pInstance;
	LockFreeQueue<Packet, 10000> m_recvPackets;
	


public :

	static CPacketDispatcher* GetInstance() {
		if (m_pInstance == nullptr)
			m_pInstance = new CPacketDispatcher;
		
		return m_pInstance;
	}
	void ClassifyPacket(CStream* _stream, CSession* _session);
	void DirectPacketProcess(CStream* _stream,CSession* _session,packettype_t _type);
	void ProcessQueuedPackets();
};
PACKET_FUNCTION_DECLARE(CLIENT_TO_SERVER_MESSAGE);
