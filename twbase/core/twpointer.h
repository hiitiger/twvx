#pragma once
#include "twobject.h"

class TW_BASE_API TwObjectGuard
{
public:
    TwObjectGuard(TwObject* obj)
        :obj_( obj)
        ,hasGuardPtr_( false)
    {
        ;
    }
    void clearGuardObject(TwObject* obj)
    {
        clearObj( obj);
    }

    void setsObj(TwObject* obj)
    {
        obj_ = obj;
    }

    static void addObj( TwObject** objaddr);
    static void removeObj( TwObject** objaddr);
    static void clearObj(TwObject* obj);

    void setHasPtr( bool i)
    {
        hasGuardPtr_ = i;
    }

    bool hasPtr() const
    {
        return hasGuardPtr_;
    }

private:
    TwObject* obj_;
    bool hasGuardPtr_;
    static std::multimap<TwObject*,TwObject**> ptrmap_;
};

template<class T = TwObject>
class TwWeakPtr
{
public:
    TwWeakPtr()
        :obj_(0)
    {
        ;
    }

    TwWeakPtr(TwObject* p)
        :obj_( p) 
    {
        TwObjectGuard::addObj(&obj_);  //record address of obj_(point to object)
    }

    TwWeakPtr(TwWeakPtr const &p)
        : obj_( p.obj_)
    {
        TwObjectGuard::addObj(&obj_);
    }

    TwWeakPtr& operator=( TwWeakPtr const & p)
    {
        if(obj_!= p.obj_)
        {
            TwObjectGuard::removeObj(&obj_);
            obj_ = p.obj_;
            TwObjectGuard::addObj(&obj_);
        }
        return * this;
    }

    TwWeakPtr& operator=( T* obj)
    {
        if(obj_!= obj)
        {
            TwObjectGuard::removeObj(&obj_);
            obj_ = obj;
            TwObjectGuard::addObj(&obj_);
        }
        return * this;
    }

    ~TwWeakPtr()
    {
        TwObjectGuard::removeObj(&obj_);
    }

    bool isNull() const
    {
        return !obj_;
    }

    operator bool() const
    {
        return obj_ != nullptr;
    }

    operator T*() const
    {
        return static_cast< T*>(obj_);
    }

    T* operator->() const
    {
        return static_cast< T*>(obj_);
    }

    T& operator*() const
    {
        return *( static_cast< T*>(obj_));
    }

    T* data() const
    {
        return static_cast< T*>(obj_);
    }

private:
    TwObject* obj_;
};
