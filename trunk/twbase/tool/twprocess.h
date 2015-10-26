#pragma once

class TW_BASE_API TwProcess
{
public:

    static DWORD getProcessId(const std::wstring& processName);
    static DWORD getProcessIdByPath(const std::wstring& processPath);

private:

};
