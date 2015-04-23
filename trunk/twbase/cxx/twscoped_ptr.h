#pragma once


template< class T >
class TwScopedPtr
{
    TW_NONCOPY(TwScopedPtr);
public:
    explicit TwScopedPtr(T* obj = nullptr)
        :obj_(obj)
    {

    }
    ~TwScopedPtr()
    {
        delete obj_;
        obj_ = nullptr;
    }

    void reset(T* obj = nullptr)
    {
        if (obj_ != obj)
        {
            delete obj_;
            obj_ = obj;
        }
    }

    T* get() const
    {
        return obj_;
    }

    T& operator*() const
    {
        DAssert(obj_);
        return *obj_;
    }

    T* operator->() const
    {
        return obj_;
    }

    operator bool() const
    {
        return obj_ != nullptr;
    }

    bool operator == (T* obj) const
    {
        return obj_ == obj;
    }

    bool operator != (T* obj) const
    {
        return obj_ != obj;
    }

private:
    T* obj_;
};

template<class T>
bool operator==(T* p1, const TwScopedPtr<T>& p2)
{
    return p1 == p2.get();
}

template<class T>
bool operator!=(T* p1, const TwScopedPtr<T>& p2)
{
    return p1 != p2.get();
}