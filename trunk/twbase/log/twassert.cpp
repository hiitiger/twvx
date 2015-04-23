#include "stable.h"
#ifdef _DEBUG

#include <process.h>
#include <crtdbg.h>

#include "twassert.h"

struct DebugAssertData
{
    const char* file;
    const char* func;
    int line;
    const char* msg;
    bool doBreak;
};

unsigned int __stdcall assertThread(void* a)
{
    DebugAssertData* d = (DebugAssertData*)a;
    int reportMode = _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
    _CrtSetReportMode(_CRT_ERROR, reportMode);
    int ret = _CrtDbgReport(_CRT_ERROR, d->file, d->line, d->func, d->msg);
    if (ret == 1)
    {
        d->doBreak = true;
    }

    return 0;
}


bool twAssert( bool cond, const char* file, const char* func, int line, const char* msg )
{
    if (cond)
    {
        return false;
    }

    DebugAssertData d = {file, func, line, msg, false};
    HANDLE handle =  (HANDLE)_beginthreadex(nullptr, 0, assertThread, (void*)&d, 0, nullptr);

    WaitForSingleObject(handle, INFINITE);

    if (d.doBreak)
    {
        DebugBreak();
    }

    return true;
}

#endif