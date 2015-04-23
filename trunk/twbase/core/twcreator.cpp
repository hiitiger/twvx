#include "stable.h"
#include "twcreator.h"
#include "twobject.h"
#include "twsingleton.h"
#include "twobjinfo.h"

TwCreator::TwCreator()
{

}

TwCreator::~TwCreator()
{

}

TwObject* TwCreator::createObject(const std::wstring& clsname )
{
    if (!exist(clsname))
    {
        return nullptr;
    }

    TwScopeLovkV1 lock(&m_classMapLock);
    return m_classMap.find(clsname)->second->createObject();
}

bool TwCreator::registerClass( const std::wstring& classname,TwClassInfo* o )
{
    if (exist(classname))
    {
        return true;
    }

    TwScopeLovkV1 lock(&m_classMapLock);
    m_classMap.insert(std::make_pair(classname, std::shared_ptr<TwClassInfo>(o)));

    return true;
}

bool TwCreator::exist( const std::wstring& classname )
{
    TwScopeLovkV1 lock(&m_classMapLock);
    return m_classMap.find(classname) != m_classMap.end();
}

TwCreator* TwCreator::twCreator()
{
    static TwCreator cc;
    return &cc;
}

TwCreator* twCreator()
{
    return TwCreator::twCreator();
}

bool twRegisterClass(const std::wstring& cls, TwClassInfo* o)
{
    return twCreator()->registerClass(cls, o);
}
