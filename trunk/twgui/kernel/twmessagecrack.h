#pragma once

#define  TW_MESSAGEMAP_BEGIN(classname) \
    BOOL xxProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res, DWORD dwMessageId = 0)\
    {\
        uMsg;wParam;lParam;res;dwMessageId;\
        switch (dwMessageId)\
        {\
        case 0:\
        {


#define  TW_MESSAGEMAP_END() \
        break;\
        }\
        default:\
            {\
                break;\
            }\
        }\
        return FALSE;\
    }


#define  TW_MESSAGE_HANDLER(msg, func)\
    if (uMsg == msg)\
    {\
        bool isHandled = func(uMsg, wParam, lParam, res);\
        if(isHandled)\
        {\
            return TRUE;\
        }\
    }

#define  TW_MESSAGE_RANGE_HANDLER(msg1, msg2, func)\
    if (uMsg >= msg1£¦£¦ uMsg <= msg2)\
    {\
        bool isHandled  = func(uMsg, wParam, lParam, res);\
        if(isHandled)\
        {\
            return TRUE;\
        }\
    }