#pragma once
#include "headers.h"
#include "Session.h"
void ReleaseServer(void);
class CIOCPServer
{
private :
	static CIOCPServer* m_pInstance;

	short m_port;
	int m_numWorkerThread;
	SOCKET m_listenSocket;
	HANDLE m_hIOCP;
	CSession* m_pSessions;

	thread* m_acceptThread;
	//vector<thread> m_vecThreads;

private:
	void InitializeSessions(int _maxClient);
	
	

public :
	static CIOCPServer* GetInstance()
	{
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CIOCPServer;
			atexit(ReleaseServer);
		}
			

		return m_pInstance;
	}
	/*----------------------------------*/
	bool AcceptThreadStart();
	bool WorkerThreadStart();
	bool ServerInitialize(short _port, int _maxClient, int _numWorkerthread);
	void Accept();
	/*----------------------------------*/

	/*----------------------------------*/
	CSession* GetSession(int _sessionID) { return &m_pSessions[_sessionID]; }
	bool DisconnectUser(int _userid);
	HANDLE GetIOCPhandle() { return m_hIOCP; }
	/*----------------------------------*/
	void Release();
	CIOCPServer();
	~CIOCPServer();
};

void AcceptThreadFunc(void);
void WorkerThreadFunc(void);

