#pragma once

#ifdef TW_DLL_GUI
#define TW_GUI_API __declspec( dllexport )
#else
#define  TW_GUI_API __declspec( dllimport )
#endif

#ifdef TW_DLL_CXX
#define TW_CXX_API __declspec( dllexport )
#else
#define  TW_CXX_API __declspec( dllimport )
#endif

#define  TWUNUSED(XXX) (XXX);

#define TW_NONCOPY(classname) \
    private:\
    classname(const classname &); \
    classname &operator=(const classname &);


// disable warning when use xsignal
#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 )


#define  TwSignal
#define  TwSlot
