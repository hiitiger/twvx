#pragma once

template<class T>
class PropertyHelper;

template<class T>
class PropertyHelper<T*>
{
public:
    typedef T* Type;

    typedef Type RType;
    typedef Type ArgType;

    static std::wstring stringType()
    {
        static std::wstring thisStringType = L"ptr";
        return thisStringType;
    }

    static RType fromString(const std::wstring& value)
    {
        std::uintptr_t ret = (std::uintptr_t)stoul(value);

        return static_cast<RType>((void*)ret);
    }

    static std::wstring toString(ArgType value)
    {
        return std::to_wstring((std::uintptr_t)value);
    }
};



template<class T>
class PropertyHelper<const T>
{
public:
    typedef T Type;

    typedef typename PropertyHelper<T>::RType RType;
    typedef typename PropertyHelper<T>::ArgType ArgType;

    static std::wstring stringType()
    {
        return PropertyHelper<T>::stringType();
    }

    static RType fromString(const std::wstring& value)
    {
        return PropertyHelper<T>::fromString(value);
    }

    static std::wstring toString(ArgType value)
    {
        return PropertyHelper<T>::toString(value);
    }
};


template<class T>
class PropertyHelper<const T*>
{
public:
    typedef T* Type;
    typedef typename PropertyHelper<T*>::RType RType;
    typedef typename PropertyHelper<T*>::ArgType ArgType;

    static std::wstring stringType()
    {
        return PropertyHelper<T*>::stringType();
    }
    static RType fromString(const std::wstring& value)
    {
        return PropertyHelper<T*>::fromString(value);
    }

    static std::wstring toString(ArgType value)
    {
        return PropertyHelper<T*>::toString(value);
    }
};

template<class T>
class PropertyHelper<const T&>
{
public:
    typedef T Type;

    typedef typename PropertyHelper<T>::RType RType;
    typedef typename PropertyHelper<T>::ArgType ArgType;

    static std::wstring stringType()
    {
        return PropertyHelper<T>::stringType();
    }

    static RType fromString(const std::wstring& value)
    {
        return PropertyHelper<T>::fromString(value);
    }

    static std::wstring toString(ArgType value)
    {
        return PropertyHelper<T>::toString(value);
    }
};

template<>
class PropertyHelper<bool>
{
public:
    typedef int Type;
    typedef Type RType;
    typedef Type ArgType;

    static std::wstring stringType()
    {
        static std::wstring thisStringType = L"bool";
        return thisStringType;
    }

    static RType fromString(const std::wstring& value) 
    {
        return value == L"true";
    }

    static std::wstring toString(ArgType value) 
    {
        return value ? L"true" : L"false";
    }
};

template<>
class PropertyHelper<int>
{
public:
    typedef int Type;
    typedef Type RType;
    typedef Type ArgType;

    static std::wstring stringType()
    {
        static std::wstring thisStringType = L"int";
        return thisStringType;
    }

    static RType fromString(const std::wstring& value) 
    {
        int ret = std::stoi(value, nullptr, 0);     
        return ret;
    }

    static std::wstring toString(ArgType value) 
    {
        return std::to_wstring(value);
    }
};

template<>
class PropertyHelper<uint32>
{
public:
    typedef uint32 Type;
    typedef Type RType;
    typedef Type ArgType;

    static std::wstring stringType()
    {
        static std::wstring thisStringType = L"uint";
        return thisStringType;
    }

    static RType fromString(const std::wstring& value) 
    {
        uint32 ret = static_cast<uint32>(std::stoul(value, nullptr, 0)); 
        return ret;
    }

    static std::wstring toString(ArgType value) 
    {
        return std::to_wstring(value);
    }
};

template<>
class PropertyHelper<int64>
{
public:
    typedef int64 Type;
    typedef Type RType;
    typedef Type ArgType;

    static std::wstring stringType()
    {
        static std::wstring thisStringType = L"int64";
        return thisStringType;
    }

    static RType fromString(const std::wstring& value) 
    {
        int64 ret = std::stoll(value, nullptr, 0);
        return ret;
    }

    static std::wstring toString(ArgType value) 
    {
        return std::to_wstring(value);
    }
};

template<>
class PropertyHelper<uint64>
{
public:
    typedef uint64 Type;
    typedef Type RType;
    typedef Type ArgType;

    static std::wstring stringType()
    {
        static std::wstring thisStringType = L"uint64";
        return thisStringType;
    }

    static RType fromString(const std::wstring& value) 
    {
        uint64 ret = std::stoull(value, nullptr, 0);
        return ret;
    }

    static std::wstring toString(ArgType value) 
    {
        return std::to_wstring(value);
    }
};


template<>
class PropertyHelper<float>
{
public:
    typedef float Type;
    typedef Type RType;
    typedef Type ArgType;

    static std::wstring stringType()
    {
        static std::wstring thisStringType = L"float";
        return thisStringType;
    }

    static RType fromString(const std::wstring& value)
    {
        float ret = std::stof(value, nullptr);
        return ret;
    }

    static std::wstring toString(ArgType value)
    {
        return std::to_wstring(value);
    }
};

template<>
class PropertyHelper<double>
{
public:
    typedef double Type;
    typedef Type RType;
    typedef Type ArgType;

    static std::wstring stringType()
    {
        static std::wstring thisStringType = L"double";
        return thisStringType;
    }

    static RType fromString(const std::wstring& value)
    {
        double ret = std::stod(value, nullptr);
        return ret;
    }
  
    static std::wstring toString(ArgType value) 
    {
        return std::to_wstring(value);
    }
};


template<>
class PropertyHelper<std::wstring>
{
public:
    typedef std::wstring Type;
    typedef Type RType;
    typedef const Type& ArgType;

    static std::wstring stringType()
    {
        static std::wstring thisStringType = L"wstring";
        return thisStringType;
    }

    static RType fromString(const std::wstring& value)
    {
        return value;
    }

    static std::wstring toString(ArgType value) 
    {
        return value;
    }
};
