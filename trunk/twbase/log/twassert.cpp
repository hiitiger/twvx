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

bool twAssert(bool cond, const char* file, const char* func, int line, const char* msg)
{
    if (cond)
    {
        return false;
    }

    DebugAssertData d = { file, func, line, msg, false };

#ifdef _DEBUG
    //      std::thread([&d](){
    int reportMode = _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
    int ret = _CrtDbgReport(_CRT_ERROR, d.file, d.line, d.func, d.msg);
    _CrtSetReportMode(_CRT_ERROR, reportMode);

    if (ret == 1)
    {
        d.doBreak = true;
    }
    //       }).join();
#endif // _DEBUG


    if (d.doBreak)
    {
        DebugBreak();
    }

    return true;
}
#endif