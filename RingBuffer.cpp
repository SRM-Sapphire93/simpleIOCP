#include "RingBuffer.h"




unsigned int CRingBuffer::Enqueue(void* pData, int nSize)
{

    if (pData == nullptr)
    {
        MoveRear(nSize);
        return 0;
    }
        

    int TempFront = m_iFront;


	if ( ((m_iRear + 1) % m_nSize) == TempFront )
		return QUEUE_ERROR_FULL;

	int ToFrontVancy = 0;
	int Enqueued = 0;
   
	//thread safe 를위해서
	
	if (m_iRear < TempFront)
	{
		ToFrontVancy = TempFront - m_iRear - 1;

		if (nSize > ToFrontVancy)
		{
			memcpy(m_pbuffer + m_iRear, pData, ToFrontVancy);
			MoveRear(ToFrontVancy);
			Enqueued += ToFrontVancy;
		}
		else
		{
			memcpy(m_pbuffer + m_iRear, pData, nSize);
			MoveRear(nSize);
			Enqueued += nSize;
		}
		return Enqueued;
	}
	else
	{
        int Left = nSize;
        int ToEndVacancy = 0;

        if (TempFront > 0)
            ToEndVacancy += 1;

        ToEndVacancy += (m_nSize - m_iRear - 1);
        if (ToEndVacancy >= Left)
        {
            memcpy(m_pbuffer + m_iRear, pData, Left);
            MoveRear(Left);
            return Left;
        }
        else
        {
            memcpy(m_pbuffer + m_iRear, pData, ToEndVacancy);
            MoveRear(ToEndVacancy);

            Left -= ToEndVacancy;
            Enqueued += ToEndVacancy;
            ToFrontVancy = TempFront - 1;

            if (ToFrontVancy < 0)
                return Enqueued;

            char* pSrc = (char*)pData + Enqueued;
            if (ToFrontVancy >= Left)
            {
                memcpy(m_pbuffer, pSrc, Left);
                MoveRear(Left);
                Enqueued += Left;
                    return Enqueued;
            }
            else
            {
                memcpy(m_pbuffer, pSrc, ToFrontVancy);
                MoveRear(ToFrontVancy);
                Enqueued += ToFrontVancy;
                return Enqueued;
            }

        }


		return Enqueued;

	}
	




}

unsigned int CRingBuffer::Dequeue(void* pData,  int nSize)
{

    if (pData == nullptr)
    {
        MoveFront(nSize);
        return 0;
    }
        

    int TempRear = m_iRear;

	if ( m_iFront == TempRear  )
		return QUEUE_ERROR_EMPTY;


	int Left = nSize;
	int Dequeued = 0;
	//thread safe 를 위해서.


	char* Src = m_pbuffer + m_iFront;
	int DistanceToEnd = 0;
	int DistanceToFront = 0;

	if (TempRear < m_iFront)
	{
		
		DistanceToEnd = m_nSize - m_iFront;
		if (Left > DistanceToEnd)
		{
			memcpy(pData, Src, DistanceToEnd);
			MoveFront(DistanceToEnd);
			Dequeued += DistanceToEnd;
			Left -= DistanceToEnd;

            DistanceToFront = TempRear;
			if (Left > DistanceToFront)
			{
				char* p = ((char*)pData + DistanceToEnd);
				memcpy(p, m_pbuffer, DistanceToFront);
				MoveFront(DistanceToFront);
				return DistanceToEnd + DistanceToFront;
			}
			else
			{
				char* p = ((char*)pData + DistanceToEnd);
				memcpy(p, m_pbuffer,Left);
				MoveFront(Left);
				return DistanceToEnd + Left;
			}
		
		}
		else
		{
			memcpy(pData, Src, Left);
			MoveFront(Left);
			Dequeued += Left;
			return Dequeued;
		}
		
	}

	else
	{
		//thread 사용시 음수가 될 가능성이있다.
		DistanceToEnd = TempRear - m_iFront;
		if (Left > DistanceToEnd)
		{
			memcpy(pData, m_pbuffer + m_iFront, DistanceToEnd);
			MoveFront(DistanceToEnd);
			return DistanceToEnd;
		}
		else
		{
			memcpy(pData, m_pbuffer + m_iFront, Left);
			MoveFront(Left);
			return Left;
		}
		

	}
	

}

unsigned int CRingBuffer::Peek(void * pData, unsigned int nOffsetFromFront)
{
	
	int TempRear = m_iRear;

	if (m_iFront == TempRear)
		return QUEUE_ERROR_EMPTY;


	int Left = nOffsetFromFront;
	int Dequeued = 0;
	//thread safe 를 위해서.


	char* Src = m_pbuffer + m_iFront;
	int DistanceToEnd = 0;
	int DistanceToFront = 0;

	if (TempRear < m_iFront)
	{

		DistanceToEnd = m_nSize - m_iFront;
		if (Left > DistanceToEnd)
		{
			memcpy(pData, Src, DistanceToEnd);
			Dequeued += DistanceToEnd;
			Left -= DistanceToEnd;

			DistanceToFront = TempRear;
			if (Left > DistanceToFront)
			{
				char* p = ((char*)pData + DistanceToEnd);
				memcpy(p, m_pbuffer, DistanceToFront);
				return DistanceToEnd + DistanceToFront;
			}
			else
			{
				char* p = ((char*)pData + DistanceToEnd);
				memcpy(p, m_pbuffer, Left);
				MoveFront(Left);
				return DistanceToEnd + Left;
			}

		}
		else
		{
			memcpy(pData, Src, Left);
			Dequeued += Left;
			return Dequeued;
		}

	}

	else
	{
		//thread 사용시 음수가 될 가능성이있다.
		DistanceToEnd = TempRear - m_iFront;
		if (Left > DistanceToEnd)
		{
			memcpy(pData, m_pbuffer + m_iFront, DistanceToEnd);
			return DistanceToEnd;
		}
		else
		{
			memcpy(pData, m_pbuffer + m_iFront, Left);
			return Left;
		}


	}
}

unsigned int CRingBuffer::GetUsage()
{
    if (IsEmpty())
        return 0;

    if (IsFull())
        return m_nSize - 1;

    int TempFront = m_iFront;
    int TempRear = m_iRear;
    
    if (TempFront > TempRear)
    {
        return (m_nSize - TempFront) + TempRear;
        
    }
    else
    {
        return (TempRear - TempFront);
    }


    
}




int CRingBuffer::GetPresentEnqueueTarget(char** pOut)
{
   
    if (IsFull())
    {
        *pOut = nullptr;
        return 0;
    }
    int tempfront = m_iFront;
    int tempRear = m_iRear;

    if (tempfront > tempRear)
    {
        *pOut = (m_pbuffer + tempRear);
        return tempfront - tempRear - 1;
    }
    else //비어있거나 Rear 가 끝부분에 도달한경우.
    {
        if (tempRear == m_nSize - 1)
        {
            *pOut = m_pbuffer + tempRear;
            return 1;
        }
        else
        {
            *pOut = (m_pbuffer + tempRear);
            return (m_nSize -1) - tempRear;
        }
    }
    return 0;
}

int CRingBuffer::GetPresentDequeueTarget(char** pOut)
{
    if (IsEmpty())
    {
        *pOut = nullptr;
        return 0;
    }
    int tempfront = m_iFront;
    int tempRear = m_iRear;

    
    if (tempfront > tempRear)
    {
        *pOut = (m_pbuffer + tempfront);
        return (m_nSize) - tempfront;
    }
    else
    {
        *pOut = (m_pbuffer + tempfront);
        return tempRear - tempfront;

    }


    return 0;

    
}

void CRingBuffer::MoveFront(int move)
{
	m_iFront = (m_iFront + move) % m_nSize;

}

void CRingBuffer::MoveRear(int move)
{
	m_iRear = (m_iRear + move) % m_nSize;


}

bool CRingBuffer::IsEmpty()
{
	if (m_iRear == m_iFront)
		return true;

	return false;
}

bool CRingBuffer::IsFull()
{
	if ( (m_iRear + 1) % m_nSize == m_iFront)
		return true;

	return false;
}

void CRingBuffer::InitOffset()
{
    m_iFront = 0; m_iRear = 0;
    return;
}

int CRingBuffer::GetVacancy()
{
    return (m_nSize -1) - GetUsage();

}

int CRingBuffer::GetFrontVacancy()
{
	return m_iFront;

}

int CRingBuffer::GetRearVacancy()
{
	return (m_nSize - 1) - m_iRear;
}



void CRingBuffer::Resize(unsigned int Size)
{
    if (m_nSize == 0)
    {
        m_pbuffer = (char*)malloc(Size);
		ZeroMemory(m_pbuffer, Size);
		m_nSize = Size;
    }
    else
    {
        m_pbuffer = (char*)realloc(m_pbuffer, Size);
		m_nSize = Size;
    }


}

CRingBuffer::CRingBuffer()
{
    Resize(10);
}
CRingBuffer::~CRingBuffer()
{
    free( m_pbuffer);
}
