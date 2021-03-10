#include "Stream.h"



void CStream::Append(const void * Data, int size)
{
    m_buffer.insert(m_buffer.end(), (char*)Data, (char*)Data + size);


}

void CStream::Clear()
{
    m_nRefCnt = 0;
    m_nOffsetOut = 0;
    m_buffer.clear();


}




void CStream::Ready_SendOrder(SendOrder & S)
{
    S.BUF.len = m_buffer.size();
    S.BUF.buf = GetData();
    S.PStream = this;
    
}

CStream & CStream::operator<<(int Data)
{
        Append(&Data, sizeof(Data));
        return *this;
        //데이터 입력.
 
}

CStream & CStream::operator>>(int & Data)
{
    Data = *( (int*)(&m_buffer[m_nOffsetOut]));
    m_nOffsetOut += sizeof(Data);
    return *this;
}

CStream & CStream::operator>>(short& Data)
{
    Data = *((short*)(&m_buffer[m_nOffsetOut]));
    m_nOffsetOut += sizeof(Data);
    return *this; 
}

CStream & CStream::operator>>(unsigned int & Data)
{
    Data = *((unsigned int*)(&m_buffer[m_nOffsetOut]));
    m_nOffsetOut += sizeof(Data);
    return *this;
}

CStream & CStream::operator>>(float & Data)
{
    Data = *((float*)(&m_buffer[m_nOffsetOut]));
    m_nOffsetOut += sizeof(Data);

    return *this;
}



CStream & CStream::operator>>(InGameInfo & Data)
{
    *this >> Data.ID;
    *this >> Data.ClassName;
    *this >> Data.FullHp;
    *this >> Data.FullMp;
    *this >> Data.MapNumber;
    *this >> Data.PosX;
    *this >> Data.PosY;
    *this >> Data.PresentHp;
    *this >> Data.PresentMp;
    *this >> Data.Direction;
    *this >> Data.WalkSpeed;
    *this >> Data.AttackSpeed;
    *this >> Data.MagicCastingSpeed;

    return *this;

}

CStream & CStream::operator<<(short & Data)
{
    Append(&Data, sizeof(Data));
    return *this;
}

CStream & CStream::operator<<(unsigned int & Data)
{
    Append(&Data, sizeof(Data));
    return *this;
}


CStream & CStream::operator<<(const wstring & Data)
{
    *this << (Data.size() * 2);
    Append(Data.data(), Data.size() * 2);
    return *this;
}
CStream & CStream::operator<<(InGameInfo & Data)
{
    *this << Data.ID;
    *this << Data.ClassName;
    *this << Data.FullHp;
    *this << Data.FullMp;
    *this << Data.MapNumber;
    *this << Data.PosX;
    *this << Data.PosY;
    *this << Data.PresentHp;
    *this << Data.PresentMp;
    *this << Data.Direction;
    *this << Data.WalkSpeed;
    *this << Data.AttackSpeed;
    *this << Data.MagicCastingSpeed;

    return *this;
}
CStream & CStream::operator<<(float & Data)
{
    Append(&Data, sizeof(Data));
    return *this;
}
CStream & CStream::operator>>(wstring & Data)
{

    int Size;
    *this >> Size;
    Data.clear();
    Data.append((WCHAR*)(&m_buffer[m_nOffsetOut]), Size / 2);
    m_nOffsetOut += Size;
    return *this;
}


