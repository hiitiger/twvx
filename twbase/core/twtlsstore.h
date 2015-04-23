#pragma once

template<class T>
class TlsStore
{
public:
    TlsStore()
    {
        index = ::TlsAlloc();
    }

    ~TlsStore()
    {
        ::TlsFree(index);
    }

    void setValue(T* v)
    {
        ::TlsSetValue(index, (void*)v);
    }

    T* getValue()
    {
        return static_cast<T*>(::TlsGetValue(index));
    }

private:
    unsigned long index;
};
