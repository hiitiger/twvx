#include "stable.h"
#include "twsysinfo.h"

bool TwSysInfo::isAfterVista()
{
    OSVERSIONINFOW osv;
    osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
    GetVersionExW(&osv);
    return osv.dwMajorVersion >= 6;
}
