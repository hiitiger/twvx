#pragma once
#include "tweventport.h"

//////////////////////////////////////////////////////////////////////////
/*
TwControl is the base class of all gui elements.
*/
class TW_GUI_API TwControl : public TwEventPort, public TwDynamicObject
{
    TW_CLASS_DECLARE(TwControl);
public:
    explicit TwControl(TwObject* parent = nullptr);
    virtual ~TwControl();

    void setName(const std::wstring& name);
    const std::wstring& name() const;

    TW_CLASSMEMEBER_BEGIN(TwControl, TwDynamicObject)
        TW_PROPERTY(TwControl, std::wstring,L"name", setName, name);
    TW_CLASSMEMEBER_END 
protected:

    std::wstring m_name;

};

TW_CLASS_REGISTER(TwControl);
