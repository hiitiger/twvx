#pragma once

class TW_BASE_API TwVar
{
public:
    TwVar();
    ~TwVar();

    TwVar(const TwVar& other);
    TwVar& operator=(const TwVar& other);

    TwVar(char v);
    TwVar(uchar v);
    TwVar(bool v);
    TwVar(int v);
    TwVar(uint v);
    TwVar(int64 v);
    TwVar(uint64 v);
    TwVar(float v);
    TwVar(double v);
    TwVar(const wchar_t* v);
    TwVar(const std::wstring& v);

    template<class T>
    static TwVar fromValue(const T& v)
    {
        TwVar var;
        var.type_ = v_var;
        VarHolderImp<T>* h = new VarHolderImp<T>(v);
        var.var_.varValue = (VarHolder*)h;
        return var;
    }

    template<class T>
    static TwVar fromValue(const T* v)
    {
        TwVar var;
        var.type_ = v_ptr;
        PtrElement* ptrElement = new PtrElementImp<typename RemoveCVR<T>::type>(static_cast<typename RemoveCVR<T>::type*>((void*)v));
        var.var_.ptrValue = ptrElement;
        return var;
    }

    template<class T>
    static TwVar fromValue(T* v)
    {
        TwVar var;
        var.type_ = v_ptr;
        PtrElement* ptrElement = new PtrElementImp<typename RemoveCVR<T>::type>(static_cast<typename RemoveCVR<T>::type*>((void*)v));
        var.var_.ptrValue = ptrElement;
        return var;
    }

    template<>
    inline static TwVar fromValue(const char& v)
    {
        return TwVar(v);
    }
    template<>
    inline static TwVar fromValue(const uchar& v)
    {
        return TwVar(v);
    }
    template<>
    inline static TwVar fromValue(const bool& v)
    {
        return TwVar(v);
    }
    template<>
    inline static TwVar fromValue(const int& v)
    {
        return TwVar(v);
    }
    template<>
    inline static TwVar fromValue(const uint& v)
    {
        return TwVar(v);
    }
    template<>
    inline static TwVar fromValue(const int64& v)
    {
        return TwVar(v);
    }
    template<>
    inline static TwVar fromValue(const uint64&v)
    {
        return TwVar(v);
    }
    template<>
    inline static TwVar fromValue(const float& v)
    {
        return TwVar(v);
    }
    template<>
    inline static TwVar fromValue(const double& v)
    {
        return TwVar(v);
    }
    template<>
    inline static TwVar fromValue(const wchar_t* const& v)
    {
        return TwVar(v);
    }
    template<>
    inline static TwVar fromValue(const std::wstring& v)
    {
        return TwVar(v);
    }

    enum  VarType
    {
        v_null,
        v_char,
        v_uchar,
        v_bool,
        v_int,
        v_uint,
        v_int64,
        v_uint64,
        v_float,
        v_double,
        v_str,
        v_ptr,
        v_var
    };

    VarType varType() const { return type_; }

    template <class T>
    operator T() const
    {
        return getObject<T>();
    }

    template<class T>
    operator T*() const
    {
        return getPtr<T>();
    }

    template<class T>
    operator const T*() const
    {
        return static_cast<const T*>(getPtr<T>());
    }

    operator void*() const
    {
        return getPtr<void>();
    }

    operator bool() const
    {
        return getbool();
    }
    operator char() const
    {
        return getChar();
    }
    operator uchar() const
    {
        return getuchar();
    }
    operator int() const
    {
        return getint();
    }
    operator uint() const
    {
        return getuint();
    }
    operator int64() const
    {
        return getint64();
    }
    operator uint64() const
    {
        return getuint64();
    }
    operator float() const
    {
        return getfloat();
    }
    operator double() const
    {
        return getdouble();
    }

    template<class T>
    T getObject()const
    {
        if (type_ == v_var)
        {
            if (var_.varValue->type() == typeid(T))
            {
                return static_cast<VarHolderImp<T>*>(var_.varValue)->var_;
            }
        }
        DAssert_M(false, "Var::getObject, error type");
        return T();
    }

    template<>
    std::wstring getObject<std::wstring>() const
    {
        if (type_ == v_str)
        {
            return *var_.strValue;
        }
        DAssert(type_ == v_str);
        return std::wstring();
    }

    template<class T>
    T* getPtr() const
    {
        if (type_ == v_ptr)
        {
            if (var_.ptrValue->type() == typeid(typename RemoveCVR<T>::type))
            {
                return static_cast<T*>(var_.ptrValue->ptr());
            }
            DAssert_M(false, "Var::getPtr, error type");
        }
        DAssert(type_ == v_ptr);
        return nullptr;
    }

    char getChar() const;

    uchar getuchar() const;

    int getint() const;

    uint getuint() const;

    int64 getint64() const;

    uint64 getuint64() const;

    float getfloat() const;

    double getdouble() const;

    bool getbool() const;

private:

    template<class T>
    TwVar(T*);

    template<class T>
    TwVar(const T&);

    void clear();

    struct VarHolder
    {
        virtual ~VarHolder()
        {

        }

        virtual VarHolder* clone() const = 0;
        virtual const std::type_info& type() const = 0;
    };

    template<class T>
    struct VarHolderImp : public VarHolder
    {
        VarHolderImp(const T& var) : var_(var) { ; }
        ~VarHolderImp() { ; }

        virtual VarHolder * clone() const
        {
            return new VarHolderImp(var_);
        }
        virtual const std::type_info& type() const
        {
            return typeid(T);
        }

        T var_;
    };

    struct PtrElement
    {
        virtual ~PtrElement()
        {

        }
        virtual PtrElement* clone() const = 0;

        virtual void* ptr() const = 0;

        virtual const std::type_info& type() const = 0;
    };

    template<class T>
    struct PtrElementImp : public PtrElement
    {
        PtrElementImp(T* ptr)
            : ptr_(ptr)
        {

        }
        virtual PtrElement* clone() const
        {
            return new PtrElementImp<T>(ptr_);
        }

        virtual void* ptr() const
        {
            return reinterpret_cast<void*>(ptr_);
        }

        virtual const std::type_info& type() const
        {
            return typeid(T);
        }

        T* ptr_;
    };


    union VarUnion
    {
        char charValue;
        uchar ucharValue;
        bool boolValue;
        int intValue;
        uint uintValue;
        int64 int64Value;
        uint64 uint64Value;
        float floatValue;
        double doubleValue;
        std::wstring* strValue;
        PtrElement* ptrValue;
        VarHolder* varValue;
    };

    VarUnion var_;
    VarType type_;
};