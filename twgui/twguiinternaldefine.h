#pragma once
#include "twguiconfig.h"

namespace TwE
{
    enum TwE_InterGlobalEventId
    {
        EventIdBegin = TwInterGlobalEventIdBegin,
        EventIdTwApp,
        EventIdAyncsTask,
        EventIdDeleteServer,

        EventIdEnd = TwInterGlobalEventIdEnd
    };

    enum TwE_AppEventId
    {
        AppEventIdBegin = TwUserEventIdBegin,
        AppEventIdEnd = TwUserEventIdEnd
    };
}

#define  WM_TW_SENDAPPEVNT WM_APP + 1