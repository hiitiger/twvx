#pragma once


/*
    do not use this in header file!!
    ** there is a situation, a global object A use B's singleton before (TwStaticSingleTon<B>)ThisSingleTonB is initialized.
	 this will cause rentry for hit in check statement:  if (InterlockedCompareExchange(&x,1,0) == 0),
	 but  statement static T* obj = new T will be executed only once for C++ constraits, so this will not cause mutli construct.
*/

//////////////////////////////////////////////////////////////////////////
template<class T>
class  TwStaticObject
{
public:
    TwStaticObject(void)
        : p(nullptr)
        , x(0)
    {

    }
    ~TwStaticObject(void)
    {

    }

    struct Deleter
    {
        Deleter(TwStaticObject& This)
        : __this(This)
        {
            ;
        }
        ~Deleter()
        {
            if (__this.p)
            {
                delete __this.p;
                __this.p = 0;
                __this.x = 0;

            }
        }
        TwStaticObject& __this;
    };

    static T* singletonInstance(TwStaticObject& thisobj)
    { 
        if (InterlockedCompareExchange(&thisobj.x, 1, 0) == 0)
        {
            static T* obj = new T;
            InterlockedExchangePointer((volatile PVOID *)&thisobj.p, (PVOID)obj);
            static Deleter ThisDelter(thisobj);
        }
        else
        {
            while (thisobj.p == nullptr)
            {
                Sleep(0);
            }
        }
        return thisobj.p;
    }

    template<const char* const>
    static T* staticInstance(TwStaticObject& thisobj)
    {
        if (InterlockedCompareExchange(&thisobj.x, 1, 0) == 0)
        {
            static T* obj = new T;
            InterlockedExchangePointer((volatile PVOID *)&thisobj.p, (PVOID)obj);
            static Deleter ThisDelter(thisobj);
        }
        else
        {
            while (thisobj.p == nullptr)
            {
                Sleep(0);
            }
        }
        return thisobj.p;
    }

    T* volatile p ;
    volatile long x ;
};


#define  TwDefine_SingleTon(Type, FUN)  \
static TwStaticObject<Type> ThisSingleTon##Type##FUN;\
static Type* FUN()\
{\
    return TwStaticObject<Type>::singletonInstance(ThisSingleTon##Type##FUN);\
}

#define  TwDefine_Static(Type, FUN)  \
static TwStaticObject<Type> ThisSingleTon##Type##FUN;\
extern const char ThisSingleTon##Type##FUN##_static_cc[] = #FUN;\
static Type* FUN()\
{\
    return TwStaticObject<Type>::staticInstance<ThisSingleTon##Type##FUN##_static_cc>(ThisSingleTon##Type##FUN);\
}


//////////////////////////////////////////////////////////////////////////
template<class T>
class  TwSingleTonBase 
{
public:
     
    static T* get()
    {
        return TwStaticObject<T>::singletonInstance(m_instance);
    }
private:
    static TwStaticObject<T> m_instance;
};

 template<class T>
    TwStaticObject<T> TwSingleTonBase<T>::m_instance;
