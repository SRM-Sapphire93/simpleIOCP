#pragma once
#include "headers.h"
#include "Net.h"
#include "RingBuffer.h"
#include "MPSCQueue.h"

class CSession
{
private : 
	int m_sessionid;

	atomic<CONST_SESSION_STATE> m_presentState;
	atomic<CONST_SESSION_IOFLAG> m_sendFlag;

	mutex m_mutex;
	SOCKET m_clntsocket;

	CRingBuffer m_recvBuffer;
	LockFreeQueue<SendOrder*, 10000> m_SendPacketQueue;
	//MPSC queue. �ټ��� �����尡 ť�� Push �Ҷ� lock free.
	//ť���� ������ ���� �ϳ��� ������ ��, send�Ϸ� �������� �Ѵ�.

	WSAOVERLAPPED m_recvOverlap;
	WSAOVERLAPPED m_sendOverlap;

public :
	void OnRecv(int _numRecvByte);
	void OnSend(int _numSendByte);
	void SendPost(CStream* _sendThisStream);
	void PostRecv();
	//void RecvPost();


	void Initialize();
	void SetSessionID(int _id) { m_sessionid = _id; }
	void SetSocket(SOCKET _socket) { m_clntsocket = _socket; }
	void SetSessionState(CONST_SESSION_STATE _state) { m_presentState = _state; }
	int GetSessionID() { return m_sessionid; }
	mutex* GetMutex() { return &m_mutex; }
	SOCKET GetSocket() { return m_clntsocket; }
	LPWSAOVERLAPPED getRecvOverlap() { return &m_recvOverlap; }
	LPWSAOVERLAPPED getSendOverlap() { return &m_sendOverlap; }
	
};




