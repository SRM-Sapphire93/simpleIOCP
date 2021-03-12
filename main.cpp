#include "IOCPServer.h"


int main()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	DWORD numThread = sysinfo.dwNumberOfProcessors * 2;
	setlocale(LC_ALL, "");

	
	CIOCPServer* pServer = CIOCPServer::GetInstance();
	pServer->ServerInitialize(6112,1000,8);
	pServer->AcceptThreadStart();
	pServer->WorkerThreadStart();
	pServer->LogicThread();
	
	

}