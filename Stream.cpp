#include "Stream.h"
#include "RingBuffer.h"

void CStream::Append(const void * Data, int size)
{
    memcpy( m_buffer + m_nOffsetIn, (char*)Data,size);
    m_nOffsetIn += size;
}

void CStream::MoveOffsetIn(int size)
{
    m_nOffsetIn += size;
}

void CStream::Clear()
{
    m_nOffsetOut = 0;
    m_nOffsetIn = 0;
}

CStream& CStream::operator<<(short& Data)
{
    Append(&Data, sizeof(Data));
    return *this;
}

CStream& CStream::operator<<(int& Data)
{
    Append(&Data, sizeof(Data));
    return *this;
}

CStream& CStream::operator<<(char& Data)
{
    Append(&Data, sizeof(Data));
    return *this;
}

CStream& CStream::operator<<(unsigned int Data)
{
    Append(&Data, sizeof(Data));
    return *this;
}

CStream& CStream::operator<<(wstring& Data)
{
    *this << (Data.size() * 2);
    Append(Data.data(), Data.size() * 2);
    return *this;
}
CStream& CStream::operator>>(string& Data)
{
    int Size = 0;
    *this >> Size;
    Data.clear();
    Data.append((char*)(&m_buffer[m_nOffsetOut]), Size);
    m_nOffsetOut += Size;
    return *this;
}
CStream& CStream::operator>>(wstring& Data)
{
    int Size = 0;
    *this >> Size;
    Data.clear();
    Data.append((WCHAR*)(&m_buffer[m_nOffsetOut]), Size / 2);
    m_nOffsetOut += Size;
    return *this;
}
CStream& CStream::operator<<(string& Data)
{
    *this << (Data.size());
    Append(Data.data(), Data.size());
    return *this;
}
CStream& CStream::operator>>(unsigned int Data)
{
    Data = *(reinterpret_cast<unsigned int*>(&m_buffer[m_nOffsetOut]));
    m_nOffsetOut += sizeof(unsigned int);
    return *this;
}
CStream& CStream::operator>>(char& Data)
{
    Data = *(reinterpret_cast<char*>(&m_buffer[m_nOffsetOut]));
    m_nOffsetOut += sizeof(char);
    return *this;
}
CStream& CStream::operator>>(int& Data)
{
    Data = *(reinterpret_cast<int*>(&m_buffer[m_nOffsetOut]));
    m_nOffsetOut += sizeof(int);
    return *this;
}
CStream& CStream::operator>>(short& Data)
{
    Data = *(reinterpret_cast<short*>(&m_buffer[m_nOffsetOut]));
    m_nOffsetOut += sizeof(short);
    return *this;
}
template<typename T> CStream& CStream::operator <<(const vector<T>& Data)
{
    unsigned int S = Data.size();
    *this << S;

    if (S == 0)
        return *this;

    char* p = (char*)Data.data();
    Append(p, sizeof(T) * S);

    return *this;
}
template <typename T>CStream& CStream::operator >>(vector<T>& Data)
{
    unsigned int S;
    *this >> S;
    for (unsigned int i = 0; i < S; ++i)
    {
        Data.push_back(   *(reinterpret_cast<T*>(m_buffer[m_nOffsetOut])) );
        m_nOffsetOut += sizeof(T);
    }

    return *this;
}



