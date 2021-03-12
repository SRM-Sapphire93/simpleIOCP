#include "Session.h"
#include "PacketDispatcher.h"

void CSession::OnRecv(int _numRecvByte)
{
	m_recvBuffer.Enqueue(nullptr, _numRecvByte);
	int recvBufferUsage = m_recvBuffer.GetUsage();
	size_t minSize = MINPACKET_SIZE;
	CPacketDispatcher* PacketDispatcher = CPacketDispatcher::GetInstance();


	while (recvBufferUsage >= minSize)
	{

		packetsize_t fullpacketSize = 0;
		m_recvBuffer.Peek(&fullpacketSize, sizeof(packetsize_t));
	
		if (recvBufferUsage >= fullpacketSize)
		{
			CStream* stream = new CStream;
			char* streamBuf = stream->GetData();

			m_recvBuffer.Dequeue(streamBuf, fullpacketSize);
			PacketDispatcher->ClassifyPacket(stream, this);

			recvBufferUsage -= fullpacketSize;
			continue;
		}
		else
			break;
	}
	if(m_recvBuffer.IsEmpty())
	m_recvBuffer.InitOffset();




}

void CSession::OnSend(int _numSendByte)
{
}

void CSession::SendPost(CStream* _sendThisStream)
{
}

void CSession::PostRecv()
{


	WSABUF wsabuf;
	char* enqueueTarget = nullptr;
	int vacancy = m_recvBuffer.GetPresentEnqueueTarget(&enqueueTarget);
	DWORD flags = 0;
	wsabuf.buf = enqueueTarget;
	wsabuf.len = vacancy;

	WSARecv(m_clntsocket, &wsabuf, 1, NULL, &flags, &m_recvOverlap, NULL);
}

void CSession::Initialize()
{
	ZeroMemory(&m_recvOverlap,sizeof(m_recvOverlap));
	ZeroMemory(&m_sendOverlap, sizeof(m_sendOverlap));
	m_recvBuffer.InitOffset();
	
	

	
}
