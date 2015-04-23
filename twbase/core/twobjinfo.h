#pragma once

struct TwNullType
{
    
};

template<class T> 
struct CCTypeCheck
{
    static const bool isNullType = false;
};

template<>
struct CCTypeCheck<TwNullType>
{
    static const bool isNullType = true;
};

//////////////////////////////////////////////////////////////////////////
class TwObject;
struct TW_BASE_API TwClassInfo 
{
    typedef TwObject* (*pfnCreate)();

    TwClassInfo(const wchar_t* clsname, const TwClassInfo* parent, pfnCreate cc)
    : m_className(clsname)
    , m_superClassInfo(parent)
    , m_creator(cc)
    {

    }
    ~TwClassInfo()
    {
        m_creator = nullptr;
        m_superClassInfo = nullptr;
    }

    TwObject* createObject()
    {
        return m_creator();
    }

    const TwClassInfo* parent() const
    {
        return m_superClassInfo;
    }
    
    bool isDerivedFrom(const TwClassInfo* d) const
    {
        for (const TwClassInfo* objData = this; objData != 0; objData = objData->parent())
        {
            if (objData == d)
            {
                return true;
            }
        }
        return false;
    }

    pfnCreate m_creator;
    std::wstring m_className;
    const TwClassInfo* m_superClassInfo;
};
//////////////////////////////////////////////////////////////////////////

#include "twobjdefine.h"