#pragma once
#include "Headers.h"
#include "Net.h"
/*---------------------------------------------------------------------*/
/*���Ź����� ������-> CStream-> CPacket -> ���� ����.
/*CPacket-> CStream -> �۽Ź����� ������
/*---------------------------------------------------------------------*/



class CStream
{
private : 

    int m_nOffsetOut = 0;
    int m_nOffsetIn = 0;
    char m_buffer[1024];

public:

    void Append(const void * data, int size);
    void MoveOffsetIn(int size);
    void Clear();
   
    
    int GetSize() { return m_nOffsetIn;}
    char* GetData() { return &(m_buffer[0]); }

   

 
    

    CStream& operator << (short& Data);
    CStream& operator << (int& Data);
    CStream& operator << (char& Data);
    CStream& operator << (unsigned int Data);
    CStream& operator <<(wstring& Data);
    CStream& operator <<(string& Data);


    CStream& operator >> (unsigned int Data);
    CStream& operator >> (char& Data);
    CStream& operator >> (int& Data);
    CStream& operator >> (short& Data);
    CStream& operator>>(string& Data);
    CStream& operator>>(wstring& Data);

    template <typename T> CStream& operator <<(const vector<T>& Data);
    template <typename T> CStream& operator >>(vector<T>& Data);

    

    CStream()
    {
        m_nOffsetIn = 0;
        m_nOffsetOut = 0;
    }
    virtual ~CStream()
    {
    }
};


