#include "stable.h"

#include "twdynamicobject.h"

TwDynamicObject::TwDynamicObject()
{

}

TwDynamicObject::~TwDynamicObject()
{

}

TwPropertySet* TwDynamicObject::thisPropertySet()
{
    static TwPropertySet sets;
    return &sets;
}

void TwDynamicObject::addProperty( TwPropertyBase* p )
{
    return TwDynamicObject::thisPropertySet()->addProperty(p);
}

bool TwDynamicObject::hasProperty( const std::wstring& name )
{
   // initInvokable();
    return TwDynamicObject::thisPropertySet()->hasProperty(name);
}

void TwDynamicObject::setStringProperty( const std::wstring&name, const std::wstring&value )
{
   // initInvokable();
    return TwDynamicObject::thisPropertySet()->setStringProperty(this, name, value);
}

std::wstring TwDynamicObject::getStringProperty( const std::wstring&name )
{
   // initInvokable();
    return TwDynamicObject::thisPropertySet()->getStringProperty(this, name);
}

void TwDynamicObject::setProperty( const std::wstring&name, const TwVar& value )
{
   // initInvokable();
    return TwDynamicObject::thisPropertySet()->setProperty(this, name, value);
}

TwVar TwDynamicObject::getProperty( const std::wstring& name )
{
  //  initInvokable();
    return TwDynamicObject::thisPropertySet()->getProperty(this, name);
}

TwMetaMethodSets* TwDynamicObject::thisMethodSet()
{
    static TwMetaMethodSets sets;
    return &sets;
}

void TwDynamicObject::addMethod( const std::wstring& name, IMetaMethod* const method )
{
    return TwDynamicObject::thisMethodSet()->addMethod(name, method);
}

bool TwDynamicObject::hasMethod( const std::wstring& name )
{
    return TwDynamicObject::thisMethodSet()->hasMethod(name);
}

TwVar TwDynamicObject::invokeMethod( const std::wstring& name )
{
    std::vector<TwVar> args;
    return invokeMethodWithArgVec(name, args);
}

TwVar TwDynamicObject::invokeMethod( const std::wstring& name, const TwVar& a1 )
{
    std::vector<TwVar> args;
    args.reserve(1);
    args.push_back(a1);
    return invokeMethodWithArgVec(name, args);
}

TwVar TwDynamicObject::invokeMethod( const std::wstring& name, const TwVar& a1, const TwVar& a2 )
{
    std::vector<TwVar> args;
    args.reserve(2);
    args.push_back(a1);
    args.push_back(a2);
    return invokeMethodWithArgVec(name, args);
}

TwVar TwDynamicObject::invokeMethod( const std::wstring& name, const TwVar& a1, const TwVar& a2, const TwVar& a3 )
{
    std::vector<TwVar> args;
    args.reserve(3);
    args.push_back(a1);
    args.push_back(a2);
    args.push_back(a3);
    return invokeMethodWithArgVec(name, args);
}

TwVar TwDynamicObject::invokeMethod( const std::wstring& name, const TwVar& a1, const TwVar& a2, const TwVar& a3, const TwVar& a4 )
{
    std::vector<TwVar> args;
    args.reserve(4);
    args.push_back(a1);
    args.push_back(a2);
    args.push_back(a3);
    args.push_back(a4);
    return invokeMethodWithArgVec( name, args);
}
TwVar TwDynamicObject::invokeMethod( const std::wstring& name, const TwVar& a1, const TwVar& a2, const TwVar& a3, const TwVar& a4, const TwVar& a5 )
{
    std::vector<TwVar> args;
    args.reserve(5);
    args.push_back(a1);
    args.push_back(a2);
    args.push_back(a3);
    args.push_back(a4);
    args.push_back(a5);
    return invokeMethodWithArgVec( name, args);
}

TwVar TwDynamicObject::invokeMethodWithArgVec( const std::wstring& name, const std::vector<TwVar>& args )
{
    initInvokable();
    return TwDynamicObject::thisMethodSet()->invokeMethod(this, name, args);
}
