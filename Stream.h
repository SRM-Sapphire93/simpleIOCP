#pragma once
#include "Headers.h"
#include "Latch.h"
/*---------------------------------------------------------------------*/
/*���Ź����� ������-> CStream-> CPacket -> ���� ����.
/*CPacket-> CStream -> �۽Ź����� ������
/*---------------------------------------------------------------------*/





class CStream
{
public : 

    int m_nOffsetOut = 0;
    vector<char> m_buffer;
    CRITICAL_SECTION m_CS;
    int m_nRefCnt = 0;


public:

    void Append(const void * data, int size);
    void Clear();
    size_t GetSize() { return m_buffer.size();}
    char* GetData() { return &(m_buffer[0]); }

    int  GetRefCnt() { return m_nRefCnt; }
    void AddRefCnt() { ++m_nRefCnt; }
    void SubRefCnt()
    {
        LOCKNOW(&m_CS)
        {
            m_nRefCnt--;
        }
    }

    void Ready_SendOrder(SendOrder& S);
   
    CStream& operator <<(int Data);
    CStream& operator <<(short& Data);
    CStream& operator <<(unsigned int& Data);
    
    CStream& operator <<(const wstring& Data);
    CStream& operator <<(InGameInfo& Data);
    CStream& operator <<(float& Data);
    template <typename T>
    CStream& operator <<(const vector<T>& Data)
    {
        unsigned int S = Data.size();
        *this << S;

        if (S == 0)
            return *this;

        char* p = (char*)Data.data();
        Append(p, sizeof(T) * S);

        return *this;
    }

    CStream& operator >>(int& Data);
    CStream& operator >>(short& Data);
    CStream& operator >>(unsigned int& Data);
    
    CStream& operator >>(InGameInfo& Data);
    CStream& operator >>(wstring& Data);
    CStream& operator >>(float& Data);

    template <typename T>
    CStream& operator >>(vector<T>& Data)
    {
        unsigned int S;
        *this >> S; 
        for (unsigned int i = 0; i < S; ++i)
        {
            Data.push_back( *( (T*)(&m_buffer[m_nOffsetOut]) ) );
            m_nOffsetOut += sizeof(T);
        }

        return *this;
    }

    

    CStream()
    {
        InitializeCriticalSection(&m_CS);
    }
    virtual ~CStream()
    {
        LOCKNOW(&m_CS)
        {

        }

        DeleteCriticalSection(&m_CS);
    }
};




