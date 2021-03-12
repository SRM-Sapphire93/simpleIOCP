#pragma once
#include "headers.h"
#define MAX_CLIENT 10000
#define MINPACKET_SIZE sizeof(packetsize_t) + sizeof(packettype_t) + 1
#define MAXPACKET_SIZE 1024
typedef char packetsize_t;
typedef char packettype_t;


enum CONST_NET_OPERATION
{
	OPERATION_RECV,
	OPERATION_SEND,
	OPERATION_ACCEPT
};
enum CONST_SESSION_STATE
{
	STATE_NOW_FREE,
	STATE_WAIT_FOR_RELEASE,
	STATE_NOW_USING,
};
enum CONST_SESSION_IOFLAG
{
	IO_SEND_PENDING,
	IO_SEND_READY_TO_POST
};

enum PACKET_TYPE
{
	CLIENT_TO_SERVER_MESSAGE,
	SERVER_TO_CLIENT_MESSAGE,
	TYPE_DISPATCH_LOCK_NEED
};

class CSession;
struct OVERLAPPEDINFORM //send or recv 는 세션의 overlapped 포인터로 결정.
{
	WSAOVERLAPPED wsaover;
	CSession* session;
};
class CStream;
struct SendOrder
{
	CStream* stream;
	atomic<int> count = 0;
	bool sessionPendingFlags[MAX_CLIENT] = { false, };
	bool DeleteOkay()
	{
		if (count.load() == 0)
			return true;
		return false;
	}
	void DeleteOrder()
	{
		count.fetch_sub(1);
		if (DeleteOkay()) //delete this;
		return;
	}
};
struct Packet
{
	CStream* stream;
	CSession* session;
	packettype_t packettpye;
};







