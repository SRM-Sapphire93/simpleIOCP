#include "IOCPServer.h"
#include "Utility.h"
#include "CommonFunction.h"
CIOCPServer* CIOCPServer::m_pInstance = nullptr;
void ReleaseServer(void)
{
	CIOCPServer* pServer = CIOCPServer::GetInstance();
	delete pServer;
}
void AcceptThreadFunc(void)
{
	CIOCPServer* pServer = CIOCPServer::GetInstance();
	while (true)
	{
		pServer->Accept();
	}

}
void WorkerThreadFunc(void)
{
	CIOCPServer* server = CIOCPServer::GetInstance();
	HANDLE iocpHandle = server->GetIOCPhandle();
	DWORD byteSucceeded = 0;
	CSession* session;
	LPOVERLAPPED overlapped;
	DWORD sendByte;
	DWORD recvByte;
	DWORD flag;

	while (true)
	{
		byteSucceeded = 0;
		session = nullptr;
		overlapped = nullptr;
		GetQueuedCompletionStatus(iocpHandle, &byteSucceeded, (PULONG_PTR)&session, &overlapped, INFINITE);

		int G = 0;
		
		if (byteSucceeded == 0 && session == nullptr && overlapped == nullptr)
			break;//PostQueuedCompletionStatus로 로직스레드 종료시 워커 스레드 종료 할수 있도록 한다.
		/*if (byteSucceeded == 0)
			server->DisconnectUser(session->GetSessionID());*/
		if (overlapped == session->getRecvOverlap())//send Order.
		{ //세션에서 send가 마무리 되었다 !
			session->OnRecv(byteSucceeded);
			session->PostRecv();
		}
		if (overlapped == session->getSendOverlap())
		{ //세션에서 recv가 마무리 되었다 !
			session->OnSend(byteSucceeded);
		}
	}
	return;
}

bool CIOCPServer::AcceptThreadStart()
{
	m_acceptThread = new thread(AcceptThreadFunc);
	cout << "Accept Start Port : " << m_port << endl;
	return true;
	
}

bool CIOCPServer::WorkerThreadStart()
{
	for (int i = 0; i < m_numWorkerThread; ++i)
		m_workerThread.push_back(new thread(WorkerThreadFunc));

	cout << "WorkerThread Start." << endl;

	return true;
}

bool CIOCPServer::LogicThread()
{
	while (true)
	{
		
	}
	for (auto ThisThread : m_workerThread)
		ThisThread->join();

}

bool CIOCPServer::ServerInitialize(short _port,int _maxClient,int _numWorkerthread)
{
	m_port = _port;
	m_numWorkerThread = _numWorkerthread;

	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	
	m_listenSocket = WSASocketW(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,NULL,WSA_FLAG_OVERLAPPED);
	NETWORKERRORCHECK(socket,m_listenSocket, INVALID_SOCKET);

	SOCKADDR_IN servAddr;
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(_port);
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	int ret = 0;

	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	ret = ::bind(m_listenSocket, (SOCKADDR*)&servAddr, sizeof(servAddr));
	NETWORKERRORCHECK(bind, ret, SOCKET_ERROR);
	ret = listen(m_listenSocket, SOMAXCONN);
	NETWORKERRORCHECK(listen, ret, SOCKET_ERROR);
	InitializeSessions(_maxClient);

	cout << "Server Initialize Succeed." << endl;


	return true;
}

void CIOCPServer::InitializeSessions(int _maxClient)
{
	m_pSessions = new CSession[_maxClient];
	for (int SessionId = 0; SessionId < _maxClient; ++SessionId)
	{
		m_pSessions[SessionId].SetSessionState(STATE_NOW_FREE);
		m_pSessions[SessionId].SetSessionID(SessionId);
	}
	return;
}

void CIOCPServer::Accept()
{
	sockaddr_in clientaddr;
	ZeroMemory(&clientaddr, sizeof(clientaddr));
	int len = sizeof(clientaddr);
	SOCKET newSocket = accept(m_listenSocket, (SOCKADDR*)(&clientaddr), &len);
	NETWORKERRORCHECK(accept, newSocket, INVALID_SOCKET);

	char bufIpAddr[256];//
	ZeroMemory(bufIpAddr, 256);

	inet_ntop(AF_INET, &clientaddr.sin_addr, bufIpAddr, 256);
	cout << "Connected User Ip : " <<  bufIpAddr<<  "Port : " << ntohs(clientaddr.sin_port)<< endl;

	m_pSessions[0].Initialize();
	m_pSessions[0].SetSocket(newSocket);
	CreateIoCompletionPort((HANDLE)newSocket, m_hIOCP, (ULONG_PTR)(&m_pSessions[0]), 0);
	m_pSessions[0].PostRecv();

	return;
	
}

bool CIOCPServer::DisconnectUser(int _userid)
{
	CSession& session = m_pSessions[_userid];
	LOCKNOW(session.GetMutex())
	{
		session.SetSessionState(STATE_WAIT_FOR_RELEASE);
		closesocket(session.GetSocket());
		session.SetSessionState(STATE_NOW_FREE);
	}
	return true;
}

void CIOCPServer::Release()
{
	WSACleanup();
}

CIOCPServer::CIOCPServer()
{

}
CIOCPServer::~CIOCPServer()
{
	Release();
}


