#pragma once
#include <map>
#include "twobject.h"

class sObjHelper
{
public:
    sObjHelper(TwObject* obj)
        :_obj( obj)
        ,_hasPtr( false)
    {
        ;
    }
    void clearPtr(TwObject* obj)
    {
        _clearObj( obj);
    }
    void setsObj(TwObject* obj)
    {
        _obj = obj;
    }
    static void _addObjToPtrmap( TwObject** objaddr);
    static void _removeObj( TwObject** objaddr);
    static void _clearObj(TwObject* obj);

    void setHasPtr( bool i)
    {
        _hasPtr = i;
    }

    bool hasPtr() const
    {
        return _hasPtr;
    }
private:
    TwObject* _obj;
    bool _hasPtr;
    static std::multimap<TwObject*,TwObject**> _ptrmap;
};

template<class T = TwObject>
class TW_GUI_API TwSafePointer
{
public:
    TwSafePointer()
        :_p(0)
    {;}

    explicit TwSafePointer(TwObject* p)
        :_p( p) 
    {
        sObjHelper::_addObjToPtrmap(&_p);  //传递的是_p的地址
    }

    TwSafePointer(TwSafePointer const &p)
        : _p( p._p)
    {
        sObjHelper::_addObjToPtrmap(&_p);
    }

    TwSafePointer& operator=( TwSafePointer const & p)
    {
        if(_p!= p._p)
        {
            sObjHelper::_removeObj(&_p);
            _p = p._p;
            sObjHelper::_addObjToPtrmap(&_p);
        }
        return * this;
    }

    ~TwSafePointer()
    {
        sObjHelper::_removeObj(&_p);
    }

    bool isNull() const
    {
        return !_p;
    }

    operator bool()
    {
        return _p != NULL;
    }

    operator T*() const
    {
        return static_cast< T*>(_p);
    }

    T* operator->() const
    {
        return static_cast< T*>(_p);
    }

    T& operator*() const
    {
        return *( static_cast< T*>(_p));
    }

    T* data() const
    {
        return static_cast< T*>(_p);
    }

private:
    TwObject* _p;  //_p是raw指针的副本，指向同一对象
};
