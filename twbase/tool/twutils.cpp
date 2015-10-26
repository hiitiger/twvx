#include "stable.h"
#include "twutils.h"
#include "twsysinfo.h"
#include <ShellAPI.h>
#include <Sddl.h>
#include <ShlObj.h>
#pragma  comment(lib, "Shell32.lib")
#pragma  comment(lib, "Advapi32.lib")

bool TwUtils::customizeUIPIPolicy(HWND window, UINT message, bool add)
{
    typedef BOOL (WINAPI* fnExType)(
        HWND                hWnd,
        UINT                message,
        DWORD               action,
        PCHANGEFILTERSTRUCT pChangeFilterStruct
        );

    typedef BOOL(WINAPI *fnType)(UINT, DWORD);


    HMODULE hModule = LoadLibrary(L"user32.dll");
    if (!hModule)
    {
        return false;
    }

    fnExType pfnEx = (fnExType)::GetProcAddress(hModule, "ChangeWindowMessageFilterEx");
    if (pfnEx)
    {
        pfnEx(window, message, add ? MSGFLT_ALLOW : MSGFLT_DISALLOW, nullptr);
    }
    else
    {
        fnType pfn = (fnType)::GetProcAddress(hModule, "ChangeWindowMessageFilter");
        if (!pfn)
        {
            FreeLibrary(hModule);
            return false;
        }

        pfn(message, add ? MSGFLT_ADD : MSGFLT_REMOVE);
    }

    FreeLibrary(hModule);
    return true;
}

void TwUtils::getWindowWorkingAreaRect(HWND window, RECT& rc)
{
    MONITORINFO mi = { 0 };
    mi.cbSize = sizeof(mi);
    HMONITOR hMonitor = ::MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
    GetMonitorInfoW(hMonitor, &mi);
    rc = mi.rcWork;
}

std::wstring TwUtils::appDataPath()
{
    WCHAR szPath[MAX_PATH];
    SHGetSpecialFolderPathW(NULL, szPath, CSIDL_LOCAL_APPDATA, TRUE);
    return szPath;
}

std::wstring  TwUtils::applicationDirPath()
{
    WCHAR szPath[MAX_PATH] = {0};
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    std::wstring dirPath = szPath;
    size_t lastSepartor = dirPath.find_last_of('\\');
    dirPath.erase(lastSepartor);
    return dirPath;
}

std::wstring  TwUtils::iexplorerPath()
{
    HKEY ieKey;
    if (ERROR_SUCCESS == RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE", REG_OPTION_NON_VOLATILE, KEY_READ, &ieKey))
    {
        WCHAR path[MAX_PATH] = { 0 };
        DWORD cbSize;
        RegQueryValueExW(ieKey, NULL, NULL, NULL, (LPBYTE)path, &cbSize);
        RegCloseKey(ieKey);
        return path;
    }
    return L"";
}

void TwUtils::makeSureDirExist(const std::wstring& dir)
{
    if (!TwFile::exist(dir))
    {
        SHCreateDirectory(NULL, dir.c_str());
    }
}

void TwUtils::removeDir(const std::wstring& dirPath)
{
    std::wstring dir = dirPath;
    dir.append(L"\\0\\0");
    SHFILEOPSTRUCTW FileOp;
    ZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCTW));

    FileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
    FileOp.hNameMappings = NULL;
    FileOp.hwnd = NULL;
    FileOp.lpszProgressTitle = NULL;
    FileOp.pFrom = dir.c_str();
    FileOp.pTo = NULL;
    FileOp.wFunc = FO_DELETE;

    SHFileOperationW(&FileOp);
}

std::wstring TwUtils::getFullPath(const std::wstring& path)
{
    std::wstring absPath;
    absPath.resize(twMax(path.size() + 1, (size_t)MAX_PATH));

    wchar_t *fileName = 0;
    DWORD retLen = GetFullPathNameW((wchar_t*)path.c_str(), (DWORD)absPath.size(), (wchar_t*)absPath.c_str(), &fileName);
    if (retLen > (DWORD)absPath.size())
    {
        absPath.resize(retLen);
        retLen = GetFullPathNameW((wchar_t*)path.c_str(), (DWORD)absPath.size(), (wchar_t*)absPath.c_str(), &fileName);
    }

    return absPath;
}

bool TwUtils::enableProcessTokenPrivilege(LPCWSTR what)
{
    int result = 0;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        TOKEN_PRIVILEGES tkp;
        tkp.PrivilegeCount = 1;
        LookupPrivilegeValueW(NULL, what, &tkp.Privileges[0].Luid);
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        result = AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, NULL);
    }
    if (hToken != NULL)
    {
        CloseHandle(hToken);
    }
    return result != 0;
}

bool TwUtils::createProcessWithNormalIL(const std::wstring& path, const std::wstring& argument)
{
    if (!TwSysInfo::isAfterVista())
    {
        return createProcess(path, argument);
    }

    BOOL fRet = FALSE;    HANDLE hToken = NULL;    HANDLE hNewToken = NULL;    PSID pIntegritySid = NULL;    TOKEN_MANDATORY_LABEL TIL = { 0 };    PROCESS_INFORMATION ProcInfo = { 0 };    STARTUPINFO StartupInfo = { 0 };
    WCHAR wszIntegritySid[20] = L"S-1-16-8192";
    std::wstring dir = path;
    dir.erase(path.find_last_of('\\'));    std::wstring cmdLine = L"\"";
    cmdLine.append(path).append(L"\" ").append(argument);    do 
    {
        fRet = OpenProcessToken(GetCurrentProcess(), TOKEN_DUPLICATE | TOKEN_ADJUST_DEFAULT | TOKEN_QUERY | TOKEN_ASSIGN_PRIMARY, &hToken);
        BREAK_IFNOT(fRet);

        fRet = DuplicateTokenEx(hToken, 0, NULL, SecurityImpersonation, TokenPrimary, &hNewToken);
        BREAK_IFNOT(fRet);

        fRet = ConvertStringSidToSid(wszIntegritySid, &pIntegritySid);
        BREAK_IFNOT(fRet);

        TIL.Label.Attributes = SE_GROUP_INTEGRITY;        TIL.Label.Sid = pIntegritySid;
        fRet = SetTokenInformation(hNewToken, TokenIntegrityLevel, &TIL, sizeof(TOKEN_MANDATORY_LABEL) + GetLengthSid(pIntegritySid));
        BREAK_IFNOT(fRet);

        fRet = CreateProcessAsUserW(hNewToken, path.c_str(), (LPWSTR)(cmdLine.c_str()), NULL, NULL, FALSE, 0, NULL, dir.c_str(), &StartupInfo, &ProcInfo);

    } while (false);

    if (ProcInfo.hProcess != NULL)    {        CloseHandle(ProcInfo.hProcess);    }
    if (ProcInfo.hThread != NULL)    {        CloseHandle(ProcInfo.hThread);    }
    if (hNewToken != NULL)    {        CloseHandle(hNewToken);    }
    if (hToken != NULL)    {        CloseHandle(hToken);    }
    LocalFree(pIntegritySid);

    return fRet != FALSE;
}

bool  TwUtils::createProcessAsDesktopUser(const std::wstring& path, const std::wstring& argument)
{
    HANDLE hExplorer = NULL;    BOOL fRet = FALSE;    HANDLE hToken = NULL;    HANDLE hNewToken = NULL;    PROCESS_INFORMATION ProcInfo = { 0 };    STARTUPINFO StartupInfo = { 0 };
    std::wstring dir = path;
    dir.erase(path.find_last_of('\\'));    std::wstring cmdLine = L"\"";
    cmdLine.append(path).append(L"\" ").append(argument);

    do
    {
        DWORD pid = TwProcess::getProcessId(L"explorer.exe");
        if (pid > 0)
        {
            hExplorer = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
        }
        BREAK_IFNOT(hExplorer);

        fRet = enableProcessTokenPrivilege(SE_INCREASE_QUOTA_NAME);
        BREAK_IFNOT(fRet);

        fRet = OpenProcessToken(hExplorer, TOKEN_DUPLICATE, &hToken);
        BREAK_IFNOT(fRet);

        fRet = DuplicateTokenEx(hToken, TOKEN_QUERY | TOKEN_ASSIGN_PRIMARY | TOKEN_DUPLICATE | TOKEN_ADJUST_DEFAULT | TOKEN_ADJUST_SESSIONID, NULL, SecurityImpersonation, TokenPrimary, &hNewToken);
        BREAK_IFNOT(fRet);

        fRet = CreateProcessAsUserW(hNewToken, path.c_str(), (LPWSTR)(cmdLine.c_str()), NULL, NULL, FALSE, 0, NULL, dir.c_str(), &StartupInfo, &ProcInfo);
        if (!fRet)
        {
            fRet = enableProcessTokenPrivilege(SE_IMPERSONATE_NAME); //default is enabled
            BREAK_IFNOT(fRet);
            typedef  BOOL (WINAPI * pfnCreateProcessWithTokenW)(
                __in        HANDLE hToken,
                __in        DWORD dwLogonFlags,
                __in_opt    LPCWSTR lpApplicationName,
                __inout_opt LPWSTR lpCommandLine,
                __in        DWORD dwCreationFlags,
                __in_opt    LPVOID lpEnvironment,
                __in_opt    LPCWSTR lpCurrentDirectory,
                __in        LPSTARTUPINFOW lpStartupInfo,
                __out       LPPROCESS_INFORMATION lpProcessInformation
                );
            pfnCreateProcessWithTokenW fn = (pfnCreateProcessWithTokenW)GetProcAddress(GetModuleHandleW(L"Advapi32.dll"), "CreateProcessWithTokenW");
            if (fn)
            {
                fRet = fn(hNewToken, 0, path.c_str(), (LPWSTR)(cmdLine.c_str()), 0, NULL, dir.c_str(), &StartupInfo, &ProcInfo);
            }
        }

    } while (false);

    if (hExplorer != NULL)
    {
        CloseHandle(hExplorer);
    }
    if (ProcInfo.hProcess != NULL)    {        CloseHandle(ProcInfo.hProcess);    }
    if (ProcInfo.hThread != NULL)    {        CloseHandle(ProcInfo.hThread);    }
    if (hNewToken != NULL)    {        CloseHandle(hNewToken);    }
    if (hToken != NULL)    {        CloseHandle(hToken);    }

    return fRet != FALSE;
}

bool  TwUtils::createProcess(const std::wstring& path, const std::wstring& argument)
{
    PROCESS_INFORMATION ProcInfo = { 0 };    STARTUPINFO StartupInfo = { 0 };

    std::wstring dir = path;
    dir.erase(path.find_last_of('\\'));
    std::wstring cmdLine = L"\"";
    cmdLine.append(path).append(L"\" ").append(argument);

    BOOL ret = CreateProcessW(path.c_str(), (LPWSTR)(cmdLine.c_str()), NULL, NULL, FALSE, 0, NULL, dir.c_str(), &StartupInfo, &ProcInfo);

    if (ProcInfo.hProcess != NULL)
    {
        CloseHandle(ProcInfo.hProcess);
    }
    if (ProcInfo.hThread != NULL)    {        CloseHandle(ProcInfo.hThread);    }

    return ret == TRUE;
}

std::wstring TwUtils::fromLocal8Bit(const char* str, int size /*= -1*/)
{
    if (size < 0)
    {
        size = (int)strlen(str);
    }
    std::wstring wstr;
    int bytesNeed = MultiByteToWideChar(CP_ACP, 0, str, size, 0, 0);
    wstr.resize(bytesNeed);
    MultiByteToWideChar(CP_ACP, 0, str, size, const_cast<wchar_t*>(wstr.c_str()), bytesNeed);
    return wstr;
}

std::string  TwUtils::toLocal8Bit(const wchar_t* wstr, int size /*= -1*/)
{
    std::string str;
    if (size < 0)
    {
        size = (int)wcslen(wstr);
    }
    int bytesNeed = WideCharToMultiByte(CP_ACP,NULL, wstr, size, NULL,0,NULL,FALSE);
    str.resize(bytesNeed);
    WideCharToMultiByte(CP_ACP,NULL, wstr, size, const_cast<char*>(str.c_str()), bytesNeed, NULL, FALSE);
    return str;
}

std::wstring TwUtils::fromUtf8(const char* str, int size /*= -1*/)
{
    std::wstring wstr;
    if (size < 0)
    {
        size = (int)strlen(str);
    }
    int bytesNeed = MultiByteToWideChar(CP_UTF8, 0, str, size, 0, 0);
    wstr.resize(bytesNeed);
    MultiByteToWideChar(CP_UTF8, 0, str, size, const_cast<wchar_t*>(wstr.c_str()), bytesNeed);
    return wstr;
}

std::string TwUtils::toUtf8(const wchar_t* wstr, int size /*= -1*/)
{
    std::string str;
    if (size < 0)
    {
        size = (int)wcslen(wstr);
    }
    int bytesNeed = WideCharToMultiByte(CP_UTF8,NULL, wstr, size, NULL,0,NULL,FALSE);
    str.resize(bytesNeed);
    WideCharToMultiByte(CP_UTF8,NULL, wstr, size, const_cast<char*>(str.c_str()), bytesNeed, NULL, FALSE);
    return str;
}

bool TwUtils::setDpiAwareNess(int what)
{
    typedef HRESULT (__stdcall* pSetDpiFunc)(int);

    HMODULE lib = LoadLibrary(L"Shcore.dll");
    if (lib)
    {
        pSetDpiFunc fn = (pSetDpiFunc)GetProcAddress(lib, "SetProcessDpiAwareness");
        if (fn)
        {
           HRESULT ret = (*fn)(what);
           return ret == S_OK;
        }
        FreeLibrary(lib);
    }
    return false;
}

std::vector<std::wstring>  TwUtils::splitString(const std::wstring& str, wchar_t delimiter)
{
    std::vector<std::wstring> internal;
    std::wstringstream ss(str); // Turn the string into a stream.
    std::wstring tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}


BOOL  TwUtils::injectLibW(DWORD dwProcessId, PCWSTR pszLibFile)
{
    BOOL bOk = FALSE; // Assume that the function fails
    HANDLE hProcess = NULL, hThread = NULL;
    PWSTR pszLibFileRemote = NULL;

    __try {
        // Get a handle for the target process.
        hProcess = OpenProcess(
            PROCESS_QUERY_INFORMATION |   // Required by Alpha
            PROCESS_CREATE_THREAD     |   // For CreateRemoteThread
            PROCESS_VM_OPERATION      |   // For VirtualAllocEx/VirtualFreeEx
            PROCESS_VM_WRITE,             // For WriteProcessMemory
            FALSE, dwProcessId);
        if (hProcess == NULL) __leave;

        // Calculate the number of bytes needed for the DLL's pathname
        int cch = 1 + lstrlenW(pszLibFile);
        int cb  = cch * sizeof(wchar_t);

        // Allocate space in the remote process for the pathname
        pszLibFileRemote = (PWSTR) 
            VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
        if (pszLibFileRemote == NULL) __leave;

        // Copy the DLL's pathname to the remote process' address space
        if (!WriteProcessMemory(hProcess, pszLibFileRemote, 
            (PVOID) pszLibFile, cb, NULL)) __leave;

        // Get the real address of LoadLibraryW in Kernel32.dll
        PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
            GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
        if (pfnThreadRtn == NULL) __leave;

        // Create a remote thread that calls LoadLibraryW(DLLPathname)
        hThread = CreateRemoteThread(hProcess, NULL, 0, 
            pfnThreadRtn, pszLibFileRemote, 0, NULL);
        if (hThread == NULL) __leave;

        // Wait for the remote thread to terminate
        WaitForSingleObject(hThread, INFINITE);

        bOk = TRUE; // Everything executed successfully
    }
    __finally { // Now, we can clean everything up

        // Free the remote memory that contained the DLL's pathname
        if (pszLibFileRemote != NULL) 
            VirtualFreeEx(hProcess, pszLibFileRemote, 0, MEM_RELEASE);

        if (hThread  != NULL) 
            CloseHandle(hThread);

        if (hProcess != NULL) 
            CloseHandle(hProcess);
    }

    return(bOk);

}
