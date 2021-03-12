#pragma once
#include "headers.h"
#define LOCKNOW(pMutex) if(CLatch latch = pMutex)
class CLatch
{
private : 
	mutex* m_mutex;
public:
	operator bool()
	{
		return true;
	}
	CLatch(mutex* _mutex)
	{
		_mutex->lock();
		m_mutex = _mutex;
	}
	~CLatch()
	{
		m_mutex->unlock();
	}

};
