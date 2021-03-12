#pragma once
#include "CommonFunction.h"

void LastError_DisplayA(const char* msg, unsigned long nMessageId)
{
    void* lpmsgbuf;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, nMessageId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&lpmsgbuf, 0, NULL);

    string fullmsg = "ERROR CODE :" + to_string(nMessageId) + " " + (char*)lpmsgbuf;

    MessageBoxA(NULL, fullmsg.c_str(), msg, MB_OK);

    LocalFree(lpmsgbuf);
    return;
}

void LastError_Display_AndQuitA(const char* msg, unsigned long nMessageId)
{
    LastError_DisplayA(msg , nMessageId);
    exit(1);
    return;
}
void LastError_DisplayW(const wchar_t* msg, unsigned long nMessageId)
{
    void* lpmsgbuf;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, nMessageId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (wchar_t*)&lpmsgbuf, 0, NULL);

    wstring fullmsg = L"ERROR CODE :" + to_wstring(nMessageId) + L" " + (wchar_t*)lpmsgbuf;

    MessageBoxW(NULL, fullmsg.c_str(), msg, MB_OK);
    LocalFree(lpmsgbuf);
    return;
}

void LastError_Display_AndQuitW(const wchar_t* msg, unsigned long nMessageId)
{
    LastError_DisplayW(msg, nMessageId);
    exit(1);
    return;
}


