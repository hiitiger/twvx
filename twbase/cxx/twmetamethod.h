#pragma once

class TwDynamicObject;


struct IMetaMethod
{
    virtual ~IMetaMethod() { ; }
    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist) = 0;
};

template<class >
struct TwMetaMethod;

template<class O>
struct TwMetaMethod<void (__thiscall O::*)()> : public IMetaMethod
{
    typedef void RType;
    typedef void (__thiscall O::*MethodType)();
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 0)
        {
            (obj->*method_)();
            return TwVar();
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
    : method_(method)
    {

    }
};


template<class O>
struct TwMetaMethod<void (__thiscall O::*)() const> : public IMetaMethod
{
    typedef void RType;
    typedef void (__thiscall O::*MethodType)() const;
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 0)
        {
            (obj->*method_)();
            return TwVar();
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};
template<class O, class R>
struct TwMetaMethod<R (__thiscall O::*)()> : public IMetaMethod
{
    typedef R RType;
    typedef R(__thiscall O::*MethodType)();
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 0)
        {
            R ret = (obj->*method_)();
            return TwVar::fromValue<typename RemoveCVR<R>::type>(ret);
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class R>
struct TwMetaMethod<R (__thiscall O::*)() const> : public IMetaMethod
{
    typedef R RType;
    typedef R(__thiscall O::*MethodType)() const;
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 0)
        {
            R ret = (obj->*method_)();
            return TwVar::fromValue<typename RemoveCVR<R>::type>(ret);
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class A1>
struct TwMetaMethod<void (__thiscall O::*)(A1)> : public IMetaMethod
{
    typedef void RType;
    typedef void (__thiscall  O::*MethodType)(A1);
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 1)
        {
            (obj->*method_)(arglist[0]);
            return TwVar();
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    } 

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};


template<class O, class A1>
struct TwMetaMethod<void ( __thiscall O::*)(A1) const> : public IMetaMethod
{
    typedef void RType;
    typedef void (__thiscall  O::*MethodType)(A1) const;
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 1)
        {
            (obj->*method_)(arglist[0]);
            return TwVar();
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    } 

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class R, class A1>
struct TwMetaMethod<R ( __thiscall O::*)(A1)> : public IMetaMethod
{
    typedef R RType;
    typedef R(__thiscall O::*MethodType)(A1);
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 1)
        {
            R ret = (obj->*method_)(arglist[0]);
            return TwVar::fromValue<typename RemoveCVR<R>::type>(ret);
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class R, class A1>
struct TwMetaMethod<R (__thiscall O::*)(A1) const> : public IMetaMethod
{
    typedef R RType;
    typedef R(__thiscall  O::*MethodType)(A1) const;
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 1)
        {
            R ret = (obj->*method_)(arglist[0]);
            return TwVar::fromValue<typename RemoveCVR<R>::type>(ret);
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class A1, class A2>
struct TwMetaMethod<void (__thiscall O::*)(A1, A2)> : public IMetaMethod
{
    typedef void RType;
    typedef void (__thiscall  O::*MethodType)(A1, A2);
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 2)
        {
           (obj->*method_)(arglist[0], arglist[1]);
           return TwVar();
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class A1, class A2>
struct TwMetaMethod<void (__thiscall O::*)(A1, A2) const> : public IMetaMethod
{
    typedef void RType;
    typedef void (__thiscall  O::*MethodType)(A1, A2) const;
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 2)
        {
            (obj->*method_)(arglist[0], arglist[1]);
            return TwVar();
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};


template<class O, class R, class A1, class A2>
struct TwMetaMethod<R (__thiscall O::*)(A1, A2)> : public IMetaMethod
{
    typedef R RType;
    typedef R(__thiscall O::*MethodType)(A1, A2);
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {  
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 2)
        {
            R ret = (obj->*method_)(arglist[0], arglist[1]);
            return TwVar::fromValue<typename RemoveCVR<R>::type>(ret);
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class R, class A1, class A2>
struct TwMetaMethod<R (__thiscall O::*)(A1, A2) const> : public IMetaMethod
{
    typedef R RType;
    typedef R(__thiscall O::*MethodType)(A1, A2) const;
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 2)
        {
            R ret = (obj->*method_)(arglist[0], arglist[1]);
            return TwVar::fromValue<typename RemoveCVR<R>::type>(ret);
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};


template<class O, class A1, class A2, class A3>
struct TwMetaMethod<void (__thiscall O::*)(A1, A2, A3)> : public IMetaMethod
{
    typedef void RType;
    typedef void (__thiscall O::*MethodType)(A1, A2, A3);
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 3)
        {
            (obj->*method_)(arglist[0], arglist[1], arglist[2]);
            return TwVar();
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    } 

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class A1, class A2, class A3>
struct TwMetaMethod<void (__thiscall O::*)(A1, A2, A3) const> : public IMetaMethod
{
    typedef void RType;
    typedef void (__thiscall O::*MethodType)(A1, A2, A3) const;
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 3)
        {
            (obj->*method_)(arglist[0], arglist[1], arglist[2]);
            return TwVar();
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    } 

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};
template<class O, class R, class A1, class A2, class A3>
struct TwMetaMethod<R (__thiscall O::*)(A1, A2, A3)> : public IMetaMethod
{
    typedef R RType;
    typedef R(O::*MethodType)(A1, A2, A3);
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 3)
        {
            R ret = (obj->*method_)(arglist[0], arglist[1], arglist[2]);
            return TwVar::fromValue<typename RemoveCVR<R>::type>(ret);
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class R, class A1, class A2, class A3>
struct TwMetaMethod<R (__thiscall O::*)(A1, A2, A3) const> : public IMetaMethod
{
    typedef R RType;
    typedef R(O::*MethodType)(A1, A2, A3) const;
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 3)
        {
            R ret = (obj->*method_)(arglist[0], arglist[1], arglist[2], arglist[3]);
            return TwVar::fromValue<typename RemoveCVR<R>::type>(ret);
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};


template<class O, class A1, class A2, class A3, class A4>
struct TwMetaMethod<void (__thiscall O::*)(A1, A2, A3, A4)> : public IMetaMethod
{
    typedef void RType;
    typedef void (__thiscall O::*MethodType)(A1, A2, A3, A4);
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 4)
        {
            (obj->*method_)(arglist[0], arglist[1], arglist[2], arglist[3]);
            return TwVar();
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    } 

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class A1, class A2, class A3, class A4>
struct TwMetaMethod<void (__thiscall O::*)(A1, A2, A3, A4) const> : public IMetaMethod
{
    typedef void RType;
    typedef void (__thiscall O::*MethodType)(A1, A2, A3, A4) const;
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 4)
        {
            (obj->*method_)(arglist[0], arglist[1], arglist[2], arglist[3]);
            return TwVar();
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    } 

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};
template<class O, class R, class A1, class A2, class A3, class A4>
struct TwMetaMethod<R (__thiscall O::*)(A1, A2, A3, A4)> : public IMetaMethod
{
    typedef R RType;
    typedef R(O::*MethodType)(A1, A2, A3, A4);
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 4)
        {
            R ret = (obj->*method_)(arglist[0], arglist[1], arglist[2], arglist[3]);
            return TwVar::fromValue<typename RemoveCVR<R>::type>(ret);
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class R, class A1, class A2, class A3, class A4>
struct TwMetaMethod<R (__thiscall O::*)(A1, A2, A3, A4) const> : public IMetaMethod
{
    typedef R RType;
    typedef R(O::*MethodType)(A1, A2, A3, A4) const;
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 4)
        {
            R ret = (obj->*method_)(arglist[0], arglist[1], arglist[2], arglist[3]);
            return TwVar::fromValue<typename RemoveCVR<R>::type>(ret);
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};


template<class O, class A1, class A2, class A3, class A4, class A5>
struct TwMetaMethod<void (__thiscall O::*)(A1, A2, A3, A4, A5)> : public IMetaMethod
{
    typedef void RType;
    typedef void (__thiscall O::*MethodType)(A1, A2, A3, A4, A5);
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 5)
        {
            (obj->*method_)(arglist[0], arglist[1], arglist[2], arglist[3], arglist[4]);
            return TwVar();
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    } 

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class A1, class A2, class A3, class A4, class A5>
struct TwMetaMethod<void (__thiscall O::*)(A1, A2, A3, A4, A5) const> : public IMetaMethod
{
    typedef void RType;
    typedef void (__thiscall O::*MethodType)(A1, A2, A3, A4, A5) const;
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 5)
        {
            (obj->*method_)(arglist[0], arglist[1], arglist[2], arglist[3], arglist[4]);
            return TwVar();
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    } 

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};
template<class O, class R, class A1, class A2, class A3, class A4, class A5>
struct TwMetaMethod<R (__thiscall O::*)(A1, A2, A3, A4, A5)> : public IMetaMethod
{
    typedef R RType;
    typedef R(O::*MethodType)(A1, A2, A3, A4, A5);
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj)
        {
            R ret = (obj->*method_)(arglist[0], arglist[1], arglist[2], arglist[3], arglist[4]);
            return TwVar::fromValue<typename RemoveCVR<R>::type>(ret);
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};

template<class O, class R, class A1, class A2, class A3, class A4, class A5>
struct TwMetaMethod<R (__thiscall O::*)(A1, A2, A3, A4, A5) const> : public IMetaMethod
{
    typedef R RType;
    typedef R(O::*MethodType)(A1, A2, A3, A4, A5) const;
    MethodType method_;

    virtual TwVar metaCall(TwDynamicObject* thisObj, const std::vector<TwVar>& arglist)
    {
        O* obj = dynamic_cast<O*>(thisObj);
        if (obj && arglist.size() == 5)
        {
            R ret = (obj->*method_)(arglist[0], arglist[1], arglist[2], arglist[3], arglist[4]);
            return TwVar::fromValue<typename RemoveCVR<R>::type>(ret);
        }
        else
        {
            DAssert_M(false, "method match error");
            return TwVar();
        }
    }

    TwMetaMethod(MethodType method)
        : method_(method)
    {

    }
};
//////////////////////////////////////////////////////////////////////////

