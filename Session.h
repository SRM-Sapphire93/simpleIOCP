#pragma once
#include "headers.h"
#include "Net.h"



class CSession;
struct OVERLAPPEDINFORM
{
	WSAOVERLAPPED wsaover;
	CONST_NET_OPERATION operation;
	CSession* session;
};


class CSession
{
private : 
	int m_sessionid;
	atomic<CONST_SESSION_STATE> m_presentState;
	mutex m_mutex;

	SOCKET m_clntsocket;
	OVERLAPPEDINFORM m_recvOverlapped;

public :
	void OnRecv(int _numRecvByte);
	void OnSend(int _numSendByte);

	void SetSessionID(int _id) { m_sessionid = _id; }
	void SetSocket(SOCKET _socket) { m_clntsocket = _socket; }
	void SetSessionState(CONST_SESSION_STATE _state) { m_presentState = _state; }
	int GetSessionID() { return m_sessionid; }
	mutex* GetMutex() { return &m_mutex; }
	SOCKET GetSocket() { return m_clntsocket; }
	OVERLAPPEDINFORM* GetOverlapped() { return &m_recvOverlapped; }
	
};




