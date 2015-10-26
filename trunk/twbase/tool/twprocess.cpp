#include "stable.h"
#include "twprocess.h"
#include <TlHelp32.h>

DWORD TwProcess::getProcessId(const std::wstring& processName)
{
    DWORD pid = -1;
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (snapshot != INVALID_HANDLE_VALUE)
    {
        if (Process32FirstW(snapshot, &entry) == TRUE)
        {
            do 
            {
                if (entry.th32ProcessID > 0)
                {
                    if (_wcsicmp(entry.szExeFile, processName.c_str()) == 0)
                    {
                        pid = entry.th32ProcessID;
                        break;
                    }
                }
            } while (Process32NextW(snapshot, &entry) == TRUE); 
        }

        CloseHandle(snapshot);
    }
   
    return pid;
}

DWORD TwProcess::getProcessIdByPath(const std::wstring& processPath)
{
    DWORD pid = -1;

    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (snapshot != INVALID_HANDLE_VALUE)
    {
        if (Process32FirstW(snapshot, &entry) == TRUE)
        {
            do 
            {
                if (entry.th32ProcessID >0)
                {
                    HANDLE hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, entry.th32ProcessID );
                    if( hModuleSnap != INVALID_HANDLE_VALUE )
                    {
                        MODULEENTRY32 me32;
                        me32.dwSize = sizeof( MODULEENTRY32W );
                        if( Module32FirstW( hModuleSnap, &me32 ) )
                        {
                            if (_wcsicmp(me32.szExePath, processPath.c_str()) == 0)
                            {
                                pid = entry.th32ProcessID;
                            }
                        }         
                        CloseHandle(hModuleSnap);
                        if (pid > 0)
                        {
                            break;
                        }
                    }
                }
               
            } while (Process32NextW(snapshot, &entry) == TRUE); 
        }
    }
   

    CloseHandle(snapshot);
    return pid;
}
