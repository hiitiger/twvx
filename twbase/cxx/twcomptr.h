#pragma once

template <class T>
class WinComPtr
{
public:
    WinComPtr()
        : p_(nullptr)
    {}

    WinComPtr (T* obj)
        : p_ (obj)
    {
        if (p_) p_->AddRef();
    }

    WinComPtr (const WinComPtr<T>& other)
        : p_ (other.p_)
    {
        if (p_) p_->AddRef();
    }

    ~WinComPtr()
    {
        release();
    }

    operator T*() const
    {
        return p_;
    }

    T& operator*() const
    {
        return *p_;
    }

    T* operator->() const
    {
        return p_;
    }

    WinComPtr& operator= (T* const newP)
    {
        if (newP != nullptr)
        {
            newP->AddRef();
        }
        release();
        p_ = newP;
        return *this;
    }

    WinComPtr& operator= (const WinComPtr<T>& newP)
    {
        return operator= (newP.p_);
    }

    T** resetAndGetPointerAddress()
    {
        release();
        p_ = nullptr;
        return &p_;
    }

#if 0
    HRESULT CoCreateInstance (REFCLSID classUUID, DWORD dwClsContext = CLSCTX_INPROC_SERVER)
    {
        HRESULT hr = ::CoCreateInstance (classUUID, 0, dwClsContext, __uuidof (T), (void**) resetAndGetPointerAddress());
        DAssert (hr != CO_E_NOTINITIALIZED); 
        return hr;
    }

    template <class OtherComClass>
    HRESULT QueryInterface (REFCLSID classUUID, WinComPtr<OtherComClass>& destObject) const
    {
        if (p_ == 0)
        {
            return E_POINTER;
        }
        return p_->QueryInterface (classUUID, (void**) destObject.resetAndGetPointerAddress());
    }

    template <class OtherComClass>
    HRESULT QueryInterface (WinComPtr<OtherComClass>& destObject) const
    {
        return this->QueryInterface (__uuidof (OtherComClass), destObject);
    }

#endif
  
private:
    T* p_;

    void release()
    {
        if (p_ != nullptr)
        {
            p_->Release();
            p_ = nullptr;
        }
    }

    T** operator&(); // private to avoid it being used accidentally
};
