#pragma once
#include "Headers.h"
#include "RingBuffer.h"

#ifdef UNICODE
#define NETWORKERRORCHECK(funcname,ret,Errret) if(ret == Errret)\
   LastError_DisplayW(TEXT(#funcname),WSAGetLastError())
#else
	#define NETWORKERRORCHECK(funcname,ret,Errret) if(ret == Errret)\
	LastError_DisplayA(TEXT(#funcname),WSAGetLastError())
#endif // UNICODE


void LastError_DisplayA(const char* msg, unsigned long nMessageId);
void LastError_Display_AndQuitA(const char* msg, unsigned long nMessageId);
void LastError_DisplayW(const wchar_t* msg, unsigned long nMessageId);
void LastError_Display_AndQuitW(const wchar_t* msg, unsigned long nMessageId);



