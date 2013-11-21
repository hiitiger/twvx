#pragma once

template<class T>
class  TwSingleTon
{
public:
    TwSingleTon(void)
        : p(NULL)
        , x(0)
    {

    }
    ~TwSingleTon(void)
    {

    }

    struct Deleter
    {
        Deleter(TwSingleTon& This)
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
        TwSingleTon& __this;
    };

    T* instance()
    { 
        if (InterlockedCompareExchange(&x,1,0) == 0)
        {
            static T* obj = new T;
            InterlockedExchangePointer(&p, (PVOID)obj);
            static Deleter ThisDelter(*this);
        }
        else
        {
            while ( p == NULL)
            {
                Sleep(0);
            }
        }
        return p;
    }

    T* volatile p ;
    volatile long x ;
};


#define  TwDefine_SingleTon(Type, FUN)  \
static Type* FUN()\
{\
    static TwSingleTon<Type> ThisSingleTon##Type;\
    return ThisSingleTon##Type.instance();\
}