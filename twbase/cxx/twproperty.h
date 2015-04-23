#pragma once
#include "twpropertyhelper.h"

class TwDynamicObject;
class TwPropertyBase
{
public:
    std::wstring name_;

    TwPropertyBase(const std::wstring& name)
        :name_(name)
    {
    }

    virtual ~TwPropertyBase() { ; }
    virtual void set(TwDynamicObject* obj, const std::wstring& value) = 0;
    virtual std::wstring get(TwDynamicObject* obj) const = 0;
    virtual void setProperty(TwDynamicObject* obj,  const TwVar& value) = 0;
    virtual TwVar getProperty(TwDynamicObject* obj) const = 0;
};

template <class O, class Ty>
class TwProperty : public TwPropertyBase
{
    std::shared_ptr<IMetaMethod> setter_;
    std::shared_ptr<IMetaMethod> getter_;
public:
    TwProperty(const std::wstring& name, const std::shared_ptr<IMetaMethod>& setterMethod, const std::shared_ptr<IMetaMethod>& getterMethod)
        : TwPropertyBase(name)
    {
        setter_ = setterMethod;
        getter_ = getterMethod;
    }

    virtual void set(TwDynamicObject* obj, const std::wstring& value)
    {
        DAssert(!value.empty());
        setProperty(obj, PropertyHelper<Ty>::fromString(value));
    }

    virtual std::wstring get(TwDynamicObject* obj) const 
    {
        return PropertyHelper<Ty>::toString(getProperty(obj));
    }

    virtual void setProperty(TwDynamicObject* obj,  const TwVar& value)
    {
        if (setter_)
        {
            std::vector<TwVar> arg(1, value);
            setter_->metaCall(obj, arg);
        }
        else
        {
            DAssert_M(false, "property is not writable");
        }
    }

    virtual TwVar getProperty(TwDynamicObject* obj) const
    {
        if (getter_)
        {
            std::vector<TwVar> arg;
            return getter_->metaCall(obj, arg);
        }
        else
        {
            DAssert_M(false, "property is not readable");
            return TwVar();
        }
    }
};


// 
// class TwObjPropertyPort
// {
// public:
// 
//     virtual ~TwObjPropertyPort() {;}
// 
//     static ObjectPropertySet* thisPropertySet()
//     {
//         static ObjectPropertySet sets;
//         return &sets;
//     }
//     
//     virtual bool initPropertySets()
//     {
//         return true;;
//     }
// 
//     void addProperty(Property* p)
//     {
//         return TwObjPropertyPort::thisPropertySet()->addProperty(p);
//     }
// 
//     virtual bool hasProperty(const std::wstring& name)
//     {
//         initPropertySets();
//         return TwObjPropertyPort::thisPropertySet()->hasProperty(name);
//     }
// 
//     virtual void setProperty(const std::wstring&name, const std::wstring&value)
//     {
//         initPropertySets();
//         return TwObjPropertyPort::thisPropertySet()->set(this, name, value);
//     }
//     virtual std::wstring getProperty(const std::wstring&name)
//     {
//         initPropertySets();
//         return TwObjPropertyPort::thisPropertySet()->get(this, name);
//     }
// 
//     void setPropertyVar(const std::wstring&name, const TwVar& value)
//     {
//         initPropertySets();
//         return TwObjPropertyPort::thisPropertySet()->setPropertyVar<TwObjPropertyPort>(this, name, value);
//     }
// 
//     TwVar getPropertyVar(const std::wstring& name)
//     {
//         initPropertySets();
//         return TwObjPropertyPort::thisPropertySet()->getPropertyVar<TwObjPropertyPort>(this, name);
//     }
// 
// };


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//do not use this

static const std::wstring k_NullPropertyType = L"NullType";
static const std::wstring k_NullPropertyName = L"NullTProperty";

class TwProperty0
{
public:
    std::wstring type_;
    std::wstring name_;
    std::wstring stringValue_;

    TwProperty0(const std::wstring& type = k_NullPropertyType, const std::wstring& name = k_NullPropertyName, const std::wstring& defStringVaule = L"")
        :type_(type), name_(name), stringValue_(defStringVaule)
    {

    }

    virtual void set(const std::wstring& value) = 0;

    virtual std::wstring get() const = 0;

};

template<class T>
class TwProperty_0 : public TwProperty0
{
public:
    T value_;
    TwProperty_0(const std::wstring& name, typename PropertyHelper<T>::ArgType value)
        : TwProperty0(PropertyHelper<T>::stringType(), name,  PropertyHelper<T>::toString(value))
        , value_(value)
    {
    }
    TwProperty_0()
        : TwProperty0(PropertyHelper<T>::stringType())
        , value_(T())
    {

    }

    virtual void set(const std::wstring& value)
    {
        value_ = PropertyHelper<T>::fromString(value);
        stringValue_ = value;
    }
    virtual std::wstring get() const 
    {
        return stringValue_;
    }

    TwProperty_0& operator=(typename PropertyHelper<T>::ArgType value)
    {
        value_ = value;
        stringValue_ = PropertyHelper<T>::toString(value);
        return *this;
    }
    operator T() const
    {
        return value_;
    }
};

class TW_BASE_API TwPropertyPort0
{
public:

    struct ProCheck
    {
        std::wstring name_;

        ProCheck(const std::wstring& name)
            :name_(name) 
        {

        }
        bool operator()(TwProperty0* p)
        {
            if (p->name_ == name_)
            {
                return true;
            }
            return false;
        }
    };

    std::set<TwProperty0*> propertys_;

    virtual ~TwPropertyPort0() {;}

    void addProperty(TwProperty0* p)
    {
        propertys_.insert(p);
    }

    void set(const std::wstring& name, const std::wstring& value)
    {
        std::set<TwProperty0*>::iterator it = std::find_if(propertys_.begin(), propertys_.end(), ProCheck(name));
        if (it != propertys_.end())
        {
            (*it)->set(value);
        }
    }

    std::wstring  get(const std::wstring& name)
    {
        std::set<TwProperty0*>::iterator it = std::find_if(propertys_.begin(), propertys_.end(),ProCheck(name));
        if (it != propertys_.end())
        {
            return   (*it)->get();
        }
        return L"";
    }
};
//
//////////////////////////////////////////////////////////////////////////