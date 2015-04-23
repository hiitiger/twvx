#pragma once

#ifdef TW_DLL_GUI
#define TW_GUI_API __declspec( dllexport )
#else
#define  TW_GUI_API __declspec( dllimport )
#endif

#ifdef TW_DLL_BASE
#define TW_BASE_API __declspec( dllexport )
#else
#define  TW_BASE_API __declspec( dllimport )
#endif

#define  TW_UNUSED(XXX) (XXX);

#define TW_NONCOPY(classname) \
    private:\
    classname(const classname &); \
    classname &operator=(const classname &);


// 
#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 )


#define  Tw_Signal
#define  Tw_Slot
