#pragma once

namespace TwUtils
{
    bool TW_BASE_API customizeUIPIPolicy(HWND window, UINT message, bool add);

    void TW_BASE_API getWindowWorkingAreaRect(HWND window, RECT& rc);

    std::wstring TW_BASE_API appDataPath();

    std::wstring TW_BASE_API applicationDirPath();

    std::wstring TW_BASE_API iexplorerPath();

    void TW_BASE_API makeSureDirExist(const std::wstring& dirPath);
    void TW_BASE_API removeDir(const std::wstring& dirPath);

    std::wstring TW_BASE_API getFullPath(const std::wstring& path);

    bool TW_BASE_API enableProcessTokenPrivilege(LPCWSTR what);
    //must be full path
    bool TW_BASE_API createProcessWithNormalIL(const std::wstring& path, const std::wstring& argument);
    bool TW_BASE_API createProcessAsDesktopUser(const std::wstring& path, const std::wstring& argument);
    bool TW_BASE_API createProcess(const std::wstring& path, const std::wstring& argument);

    bool TW_BASE_API setDpiAwareNess(int);

    std::wstring TW_BASE_API fromLocal8Bit(const char* str, int size = -1);
    std::string TW_BASE_API toLocal8Bit(const wchar_t* wstr, int size = -1);
    std::wstring TW_BASE_API fromUtf8(const char* str, int size = -1);
    std::string TW_BASE_API toUtf8(const wchar_t* str, int size = -1);

    std::vector<std::wstring> TW_BASE_API splitString(const std::wstring&, wchar_t delimiter);
    
    BOOL TW_BASE_API injectLibW(DWORD dwProcessId, PCWSTR pszLibFile) ;

}