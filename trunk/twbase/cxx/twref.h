#pragma once


template<class C>
class TwRefObject : public C
{
    TW_DISABLE_COPY(TwRefObject);
public:
    TwRefObject()
        :ref_(0)
    {
        ;
    }

    virtual ~TwRefObject() 
    { 
        DAssert(ref_ == 0);
    }

    int addRef()
    {
        _InterlockedIncrement((volatile LONG*)&ref_);
        return ref_;
    }

    int release()
    {
        _InterlockedDecrement((volatile LONG*)&ref_);
        if (ref_ == 0)
        {
            delete this;
            return 0;
        }
        return ref_;
    }

    int refCount()
    {
        return ref_;
    }

    volatile int ref_;
};


template<class T>
class TwRefPtr
{
public:
    TwRefPtr()
        : obj_(nullptr)
    {

    }

    TwRefPtr(T* obj)
        : obj_(obj)
    {
        if (obj_)
        {
            obj_->addRef();
        }
    }

    ~TwRefPtr()
    {
        if (obj_)
        {
            obj_->release();
        }
    }

    TwRefPtr(const TwRefPtr<T>& r)
        : obj_(r.obj_)
    {
        if (obj_)
        {
            obj_->addRef();
        }
    }

    template<class U>
    TwRefPtr(const TwRefPtr<U>& r)
        : obj_(r.get())
    {
        if (obj_)
        {
            obj_->addRef();
        }
    }

    T* get() const 
    {
        return obj_;
    }

    operator T*() const 
    {
        return obj_;
    }

    T* operator->() const 
    {
        return obj_;
    }
    
    operator bool () const
    {
        return obj_ != nullptr;
    }

    TwRefPtr<T>& operator = (T* p)
    {
        if (obj_ == p)
        {
            return *this;
        }

        if(obj_ )
        {
            obj_->release();
        }

        obj_ = p;
        if(obj_)
        {
            obj_->addRef();
        }

        return *this;
    }

    TwRefPtr<T>& operator = (const TwRefPtr<T>& r)
    {
        return *this = r.obj_;
    }

    template<class U>
    TwRefPtr<T>& operator = (const TwRefPtr<U>& r)
    {
        return *this = r.get();
    }

    int refCount()
    {
        return obj_->refCount();
    }

private:
    T* obj_;
};
