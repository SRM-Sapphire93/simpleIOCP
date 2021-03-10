#pragma once
#include "Headers.h"
#include <string>
enum CONST_RINGBUFFER_ERROR
{
	QUEUE_ERROR_FULL = -29,
    QUEUE_ERROR_EMPTY,
};


class CRingBuffer
{
private : 
     int m_nSize = 0;
     int m_iFront = 0;
     int m_iRear = 0;
	 int m_nUsage = 0;

     char* m_pbuffer;
    

	
public:
    
	//Push to queue 
	unsigned int Enqueue(void* pData,  int nSize);
	//Pop From queue
	unsigned int Dequeue(void* pData,  int nSize);
	//Peek 
    unsigned int Peek(void* pData, unsigned int nOffsetFromFront);
    unsigned int GetUsage();

    int GetPresentEnqueueTarget(char** pOut);
    int GetPresentDequeueTarget(char** pOut);

	void MoveFront(int move);
	void MoveRear(int move);
	


	bool IsEmpty();    
	bool IsFull();

    void InitOffset();
	int GetVacancy();
	int GetFrontVacancy();
	int GetRearVacancy();



    void Resize(unsigned int Size); 
    CRingBuffer();
    virtual ~CRingBuffer();
};

