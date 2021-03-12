#include "PacketDispatcher.h"
#include "Session.h"

CPacketDispatcher* CPacketDispatcher::m_pInstance = nullptr;





void CPacketDispatcher::ClassifyPacket(CStream * _stream, CSession * _session)
{
	packetsize_t packetsize = 0;
	packettype_t packettype = 0;
	
	CStream& stream = *_stream;
	stream >> packetsize;
	stream >> packettype;

	if (packettype < TYPE_DISPATCH_LOCK_NEED)
		DirectPacketProcess(_stream,_session, packettype);

	else
	{
		Packet packet;
		packet.session = _session;
		packet.stream = _stream;
		packet.packettpye = packettype;
		m_recvPackets.Enqueue(packet);
	}
		



}
void CPacketDispatcher::DirectPacketProcess(CStream* _packet, CSession* _session, packettype_t _type)
{
	
	switch (_type)
	{
		CASE_PACKET_FUNCTION(_packet, _session,CLIENT_TO_SERVER_MESSAGE);

	default:
		break;
	}


}


void CPacketDispatcher::ProcessQueuedPackets()
{
}

PACKET_FUNCTION_DECLARE(CLIENT_TO_SERVER_MESSAGE)
{

	CStream& stream = *_packet;
	CSession& session = *_session;
	string message;
	stream >> message;

	cout << "from Client : " << message.c_str() << endl;


}




