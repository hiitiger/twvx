#include "stable.h"
#include <stdio.h>
#include "debugconsole.h"


DebugConsole::DebugConsole(void)
{
#ifdef _DEBUG
#pragma warning(disable:4996)
    AllocConsole();   
    SetConsoleTitleW(L"Debug Console");
    freopen("conin$", "r+t", stdin);   
    freopen("conout$", "w+t", stdout);   
    freopen("conout$", "w+t", stderr);
#endif // _DEBUG


}

DebugConsole::~DebugConsole(void)
{
#ifdef _DEBUG
    fclose(stderr);   
    fclose(stdout);   
    fclose(stdin);   
    FreeConsole();
#endif // _DEBUG
}

DebugConsole* DebugConsole::getConsole()
{
    static DebugConsole sc;
    return &sc;
}

