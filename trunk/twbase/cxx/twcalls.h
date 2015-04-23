#pragma once

#include "twobjecttraits.h"

#define ArgTraits(A) const A &

// 
// template<class T, class Mfn, class L>
// void TwAsyncCall(T* obj, Mfn fn, L* loop)
// {
//     std::function<void()> func = [=]
//     {
//         (obj->*fn)();
//     };
// 
//     loop->postFunc(func);
// }
// 
// template<class T, class Mfn, class A1, class L>
// void TwAsyncCall(T* obj, Mfn fn, A1 a1, L* loop)
// {
//     std::function<void()> func = [=]
//     {
//         (obj->*fn)(a1);
//     };
// 
//     loop->postFunc(func);
// }
// 
// template<class T, class Mfn, class A1, class A2, class L>
// void TwAsyncCall(T* obj, Mfn fn, A1 a1, A2 a2, L* loop)
// {
//     std::function<void()> func = [=]
//     {
//         (obj->*fn)(a1, a2);
//     };
// 
//     loop->postFunc(func);
// }
// 
// template<class T, class Mfn, class A1,class A2, class A3, class L>
// void TwAsyncCall(T* obj, Mfn fn, A1 a1, A2 a2, A3 a3, L* loop)
// {
//     std::function<void()> func = [=]
//     {
//         (obj->*fn)(a1, a2, a3);
//     };
// 
//     loop->postFunc(func);
// }
// 
// template<class T, class Mfn, class A1,class A2, class A3,class A4, class L>
// void TwAsyncCall(T* obj, Mfn fn, A1 a1, A2 a2, A3 a3, A4 a4, L* loop)
// {
//     std::function<void()> func = [=]
//     {
//         (obj->*fn)(a1, a2, a3, a4);
//     };
// 
//     loop->postFunc(func);
// }
// 
// template<class T, class Mfn, class A1,class A2, class A3, class A4, class A5, class L>
// void TwAsyncCall(T* obj, Mfn fn, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, L* loop)
// {
//     std::function<void()> func = [=]
//     {
//         (obj->*fn)(a1, a2, a3, a4, a5);
//     };
// 
//     loop->postFunc(func);
// }
// 
// template<class T, class Mfn, class A1,class A2, class A3, class A4, class A5, class A6, class L>
// void TwAsyncCall(T* obj, Mfn fn, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, L* loop)
// {
//     std::function<void()> func = [=]
//     {
//         (obj->*fn)(a1, a2, a3, a4, a5, a6);
//     };
// 
//     loop->postFunc(func);
// }


//////////////////////////////////////////////////////////////////////////
template<class T, class Mfn, class TObjHolder = TwHolderTraits<T>::TwHolder >
struct TwObjAsyncCallTask0 : public TwTask
{
    TwObjAsyncCallTask0(T* obj, Mfn fn)
        :obj_(obj), fn_(fn)
    {
    }

    virtual void run() 
    {
        if (obj_)
        {
            (getObj(obj_)->*fn_)();
        }
    }

    TObjHolder obj_;
    Mfn fn_;
};

template<class T, class Mfn>
TwTask* TwMakeCallTask(T* obj, Mfn fn)
{
    return static_cast<TwTask*>(new TwObjAsyncCallTask0<T, Mfn >(obj, fn));
}

template<class T, class Mfn, class L>
void TwAsyncCall(T* obj, Mfn fn, L* loop)
{
    loop->postTask(TwMakeCallTask(obj, fn));
}

template<class T, class Mfn, class A1, class TObjHolder = TwHolderTraits<T>::TwHolder >
struct TwObjAsyncCallTask1 : public TwTask
{
    TwObjAsyncCallTask1(T* obj, Mfn fn, ArgTraits(A1) a1)
        :obj_(obj), fn_(fn), a1_(a1)
    {

    }

    virtual void run() 
    {
        if (obj_)
        {
            (getObj(obj_)->*fn_)(a1_);
        }
    }
    TObjHolder obj_;
    Mfn fn_;
    A1 a1_;
};

template<class T, class Mfn, class A1 >
TwTask* TwMakeCallTask(T* obj, Mfn fn, ArgTraits(A1) a1)
{
    return static_cast<TwTask*>(new TwObjAsyncCallTask1<T, Mfn, A1>(obj, fn, a1));
}
 
template<class T, class Mfn, class A1, class L>
void TwAsyncCall(T* obj, Mfn fn, ArgTraits(A1) a1, L* loop)
{
    loop->postTask(TwMakeCallTask(obj, fn, a1));
}

template<class T, class Mfn, class A1, class A2 , class TObjHolder = TwHolderTraits<T>::TwHolder >
struct TwObjAsyncCallTask2 : public TwTask
{
    TwObjAsyncCallTask2(T* obj, Mfn fn, ArgTraits(A1) a1, ArgTraits(A2) a2)
        :obj_(obj), fn_(fn), a1_(a1), a2_(a2)
    {
    }

    virtual void run() 
    {
        if (obj_)
        {
            (getObj(obj_)->*fn_)(a1_, a2_);
        }
    }
   TObjHolder obj_;
    Mfn fn_;
    A1 a1_;
    A2 a2_;
};

template<class T, class Mfn, class A1, class A2>
TwTask* TwMakeCallTask(T* obj, Mfn fn, ArgTraits(A1) a1, ArgTraits(A2) a2 )
{
    return static_cast<TwTask*>(new TwObjAsyncCallTask2<T, Mfn, A1, A2>(obj, fn, a1, a2));
}

template<class T, class Mfn, class A1, class A2, class L>
void TwAsyncCall(T* obj, Mfn fn, ArgTraits(A1) a1, ArgTraits(A2) a2, L* loop)
{
    loop->postTask(TwMakeCallTask(obj, fn, a1, a2));
}

template<class T, class Mfn, class A1,class A2, class A3,  class TObjHolder = TwHolderTraits<T>::TwHolder>
struct TwObjAsyncCallTask3 : public TwTask
{
    TwObjAsyncCallTask3(T* obj, Mfn fn, ArgTraits(A1) a1, ArgTraits(A2) a2, ArgTraits(A3) a3)
        :obj_(obj), fn_(fn), a1_(a1), a2_(a2), a3_(a3)
    {
      
    }

    virtual void run() 
    {
        if (obj_)
        {
            (getObj(obj_)->*fn_)(a1_, a2_, a3_);
        }
    }
    TObjHolder obj_;
    Mfn fn_;
    A1 a1_;
    A2 a2_;
    A3 a3_;
};

template<class T, class Mfn, class A1, class A2, class A3>
TwTask* TwMakeCallTask(T* obj, Mfn fn, ArgTraits(A1) a1, ArgTraits(A2) a2 ,  ArgTraits(A3) a3)
{
    return static_cast<TwTask*>(new TwObjAsyncCallTask3<T, Mfn, A1, A2, A3>(obj, fn, a1, a2, a3));
}

template<class T, class Mfn, class A1, class A2, class A3, class L>
void TwAsyncCall(T* obj, Mfn fn, ArgTraits(A1) a1, ArgTraits(A2) a2, ArgTraits(A3) a3, L* loop)
{
    loop->postTask(TwMakeCallTask(obj, fn, a1, a2, a3));
}

template<class T, class Mfn, class A1,class A2, class A3, class A4, class TObjHolder = TwHolderTraits<T>::TwHolder>
struct TwObjAsyncCallTask4 : public TwTask
{
    TwObjAsyncCallTask4(T* obj, Mfn fn, ArgTraits(A1) a1, ArgTraits(A2) a2, ArgTraits(A3) a3, ArgTraits(A4) a4)
        :obj_(obj), fn_(fn), a1_(a1), a2_(a2), a3_(a3), a4_(a4)
    {
        
    }

    virtual void run() 
    {
        if (obj_)
        {
            (getObj(obj_)->*fn_)(a1_, a2_, a3_, a4_);
        }
    }
    TObjHolder obj_;
    Mfn fn_;
    A1 a1_;
    A2 a2_;
    A3 a3_;
    A4 a4_;
};

template<class T, class Mfn, class A1, class A2, class A3, class A4>
TwTask* TwMakeCallTask(T* obj, Mfn fn, ArgTraits(A1) a1, ArgTraits(A2) a2 ,  ArgTraits(A3) a3,  ArgTraits(A4) a4)
{
    return static_cast<TwTask*>(new TwObjAsyncCallTask4<T, Mfn, A1, A2, A3, A4>(obj, fn, a1, a2, a3, a4));
}

template<class T, class Mfn, class A1,class A2, class A3,class A4, class L>
void TwAsyncCall(T* obj, Mfn fn,  ArgTraits(A1) a1, ArgTraits(A2) a2, ArgTraits(A3) a3, ArgTraits(A4) a4, L* loop)
{
    loop->postTask(TwMakeCallTask(obj, fn, a1, a2, a3, a4));
}

template<class T, class Mfn, class A1,class A2, class A3, class A4, class A5, class TObjHolder = TwHolderTraits<T>::TwHolder>
struct TwObjAsyncCallTask5 : public TwTask
{
    TwObjAsyncCallTask5(T* obj, Mfn fn, ArgTraits(A1) a1, ArgTraits(A2) a2, ArgTraits(A3) a3, ArgTraits(A4) a4, ArgTraits(A5) a5)
        :obj_(obj), fn_(fn), a1_(a1), a2_(a2), a3_(a3), a4_(a4), a5_(a5)
    {
      
    }

    virtual void run() 
    {
        if (obj_)
        {
            (getObj(obj_)->*fn_)(a1_, a2_, a3_, a4_, a5_);
        }
    }
    TObjHolder obj_;
    Mfn fn_;
    A1 a1_;
    A2 a2_;
    A3 a3_;
    A4 a4_;
    A5 a5_;
};

template<class T, class Mfn, class A1,class A2, class A3, class A4, class A5>
TwTask* TwMakeCallTask(T* obj, Mfn fn, ArgTraits(A1) a1, ArgTraits(A2) a2 ,  ArgTraits(A3) a3,  ArgTraits(A4) a4, ArgTraits(A5) a5)
{
    return static_cast<TwTask*>(new TwObjAsyncCallTask5<T, Mfn, A1, A2, A3, A4, A5>(obj, fn, a1, a2, a3, a4, a5));
}

template<class T, class Mfn, class A1,class A2, class A3, class A4, class A5, class L>
void TwAsyncCall(T* obj, Mfn fn, ArgTraits(A1) a1, ArgTraits(A2) a2, ArgTraits(A3) a3, ArgTraits(A4) a4, ArgTraits(A5) a5, L* loop)
{
    loop->postTask(TwMakeCallTask(obj, fn, a1, a2, a3, a4, a5));
}

