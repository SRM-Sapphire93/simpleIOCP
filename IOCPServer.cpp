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
		cout << "so good" << endl;
		pServer->Accept();
	}

}
void WorkerThreadFunc(void)
{
	while (true)
	{
		
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
		//m_vecThreads.emplace_back(WorkerThreadFunc);

	return true;
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

	ret = ::bind(m_listenSocket, (SOCKADDR*)&servAddr, sizeof(servAddr));
	NETWORKERRORCHECK(bind, ret, SOCKET_ERROR);
	ret = listen(m_listenSocket, SOMAXCONN);
	NETWORKERRORCHECK(listen, ret, SOCKET_ERROR);

	//m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
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


