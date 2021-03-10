#pragma once
#include <atomic>

template<class T , size_t size>
class LockFreeQueue
{
public:
	LockFreeQueue() { memset(m_flags, false, bufSize); }

	bool Enqueue(const T& Element)
	{
		while (true)
		{
			int oldWritePos = m_writePos.load();
			int nextWritePos = GetPositionAfter(oldWritePos);

			if (nextWritePos == m_readPos)
				return false; //queue full.

			if (m_writePos.compare_exchange_strong(oldWritePos, nextWritePos))
			{
				m_buffer[oldWritePos] = Element;
				m_flags[oldWritePos] = true;
				return true;
			}
		}
	}
	bool Dequeue(T* dest)
	{
		int oldReadPos = m_readPos;
		int oldWritePos = m_writePos.load();
		
		if (oldReadPos == oldWritePos)
			return false;

		if (m_flags[oldReadPos] == true)
		{
			*dest = m_buffer[oldReadPos];
			m_flags[oldReadPos] = false;
			m_readPos = GetPositionAfter(oldReadPos);
			return true;
		}
		return false;
	}
	bool IsEmpty()
	{
		if (m_readPos == m_writePos)
			return true;

		return false;
	}
	
private:
	int GetPositionAfter(int Pos)
	{
		return ++Pos == bufSize ? 0 : Pos;
	}


private :
	static constexpr int bufSize = size + 1;
	
	T m_buffer[bufSize];
	bool m_flags[bufSize];
	std::atomic<int> m_writePos = 0;
	
	int m_readPos = 0;
};