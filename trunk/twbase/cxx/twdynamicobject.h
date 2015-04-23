#pragma once

class TW_BASE_API TwPropertySet
{
public:
    std::map<std::wstring , std::shared_ptr<TwPropertyBase> > propertys_;

    virtual ~TwPropertySet() {;}

    void addProperty(TwPropertyBase* p)
    {
        DAssert(propertys_.find(p->name_) ==  propertys_.end());
        propertys_.insert(std::make_pair(p->name_, std::shared_ptr<TwPropertyBase>(p)));
    }

    bool hasProperty(const std::wstring& name)
    {
        auto it = propertys_.find(name);
        return it!=propertys_.end();
    }

    void setStringProperty(TwDynamicObject* obj, const std::wstring& name, const std::wstring& value)
    {
        auto it = propertys_.find(name);
        if (it != propertys_.end())
        {
            it->second->set(obj, value);
            return;
        }
        DAssert_M(false, "set wrong properyty");
    }

    std::wstring  getStringProperty(TwDynamicObject* obj, const std::wstring& name)
    {
        auto it = propertys_.find(name);
        if (it != propertys_.end())
        {
            return it->second->get(obj);
        }
        DAssert_M(false, "get wrong properyty");
        return L"";
    }

    TwVar getProperty(TwDynamicObject* obj, const std::wstring& name)
    {
        auto it = propertys_.find(name);
        if (it != propertys_.end())
        {
            return it->second->getProperty(obj);
        }
        DAssert_M(false, "ObjectPropertySet::getPropertyTy error property name");
        return TwVar();
    }

    void setProperty(TwDynamicObject* obj, const std::wstring& name, const TwVar& value)
    {
        auto it = propertys_.find(name);
        if (it != propertys_.end())
        {
            it->second->setProperty(obj, value);
            return;
        }

        DAssert_M(false, "ObjectPropertySet::setPropertyTy error property name");
    }
};


class TW_BASE_API TwMetaMethodSets
{
public:
    std::map<std::wstring, std::shared_ptr<IMetaMethod> > methods_;

    bool hasMethod(const std::wstring& name)
    {
        return methods_.find(name) != methods_.end();
    }

    void addMethod(const std::wstring& name, IMetaMethod* const method)
    {
        DAssert_M(!hasMethod(name), "already add this method");
        methods_.insert(std::make_pair(name, std::shared_ptr<IMetaMethod>(method)));
    }

    std::shared_ptr<IMetaMethod> findMethod(const std::wstring& name)
    {
        auto it = methods_.find(name);
        if (it != methods_.end())
        {
            return it->second;
        }
        return nullptr;
    }

    TwVar invokeMethod(TwDynamicObject* obj, const std::wstring& name, const std::vector<TwVar>& args)
    {
        auto it = methods_.find(name);
        if(it != methods_.end())
        {
            return it->second->metaCall(obj, args);
        }
        else
        {
            DAssert_M(false, "call wrong method");
            return TwVar();
        }
    }
};

class TW_BASE_API TwDynamicObject 
{
public:
    TwDynamicObject();
    virtual ~TwDynamicObject();

    virtual bool initInvokable() { return true; }

    static TwPropertySet* thisPropertySet();
    void addProperty(TwPropertyBase* p);
    virtual bool hasProperty(const std::wstring& name);
    virtual void setStringProperty(const std::wstring&name, const std::wstring&value);
    virtual std::wstring getStringProperty(const std::wstring&name);
    virtual void setProperty(const std::wstring&name, const TwVar& value);
    virtual TwVar getProperty(const std::wstring& name);

    static TwMetaMethodSets* thisMethodSet();
    void addMethod(const std::wstring& name, IMetaMethod* const method);
    virtual bool hasMethod(const std::wstring& name);
    virtual TwVar invokeMethodWithArgVec(const std::wstring& name, const std::vector<TwVar>& args);

    TwVar invokeMethod(const std::wstring& name);
    TwVar invokeMethod(const std::wstring& name, const TwVar& a1);
    TwVar invokeMethod(const std::wstring& name, const TwVar& a1, const TwVar& a2);
    TwVar invokeMethod(const std::wstring& name, const TwVar& a1, const TwVar& a2, const TwVar& a3);
    TwVar invokeMethod(const std::wstring& name, const TwVar& a1, const TwVar& a2, const TwVar& a3, const TwVar& a4);
    TwVar invokeMethod(const std::wstring& name, const TwVar& a1, const TwVar& a2, const TwVar& a3, const TwVar& a4, const TwVar& a5);

};


//////////////////////////////////////////////////////////////////////////


#define TW_SUPPORTPROPERTY(ClsType, PClsType)\
public:\
    static TwPropertySet* thisPropertySet()\
    {\
        static TwPropertySet sets;\
        return &sets;\
    }\
    void addProperty(TwPropertyBase* p)\
    {\
        return ClsType::thisPropertySet()->addProperty(p);\
    }\
    virtual bool hasProperty(const std::wstring& name)\
    {\
        initInvokable();\
        if (ClsType::thisPropertySet()->hasProperty(name))\
        {\
            return true;\
        }\
        return PClsType::hasProperty(name);\
    }\
    virtual void setStringProperty(const std::wstring&name, const std::wstring&value)\
    {\
        initInvokable();\
        if (ClsType::thisPropertySet()->hasProperty(name))\
        {\
            return  ClsType::thisPropertySet()->setStringProperty(this, name, value);\
        }\
        return PClsType::setStringProperty( name, value);\
    }\
    virtual std::wstring getStringProperty(const std::wstring&name)\
    {\
        initInvokable();\
        if (ClsType::thisPropertySet()->hasProperty(name))\
        {\
            return  ClsType::thisPropertySet()->getStringProperty(this, name);\
        }\
        return PClsType::getStringProperty(name);\
    }\
    void setProperty(const std::wstring&name, const TwVar& value)\
    {\
        initInvokable();\
        if (ClsType::thisPropertySet()->hasProperty(name))\
        {\
            return ClsType::thisPropertySet()->setProperty(this, name, value);\
        }\
        PClsType::setProperty( name, value);\
    }\
    TwVar getProperty(const std::wstring& name)\
    {\
        initInvokable();\
        if (ClsType::thisPropertySet()->hasProperty(name))\
        {\
            return ClsType::thisPropertySet()->getProperty(this, name);\
        }\
        return PClsType::getProperty( name);\
    }\

#define  TW_SUPPORTINVOKEMETHOD(ClsType, PClsType)\
static TwMetaMethodSets* thisMethodSet()\
{\
    static TwMetaMethodSets sets;\
    return &sets;\
}\
void addMethod(const std::wstring& name, IMetaMethod* const method)\
{\
    ClsType::thisMethodSet()->addMethod(name, method);\
}\
virtual bool hasMethod(const std::wstring& name)\
{\
    initInvokable();\
    if (ClsType::thisMethodSet()->hasMethod(name))\
    {\
        return true;\
    }\
    return PClsType::hasMethod(name);\
}\
virtual TwVar invokeMethodWithArgVec(const std::wstring& name, const std::vector<TwVar>& args)\
{\
    initInvokable();\
    if (ClsType::thisMethodSet()->hasMethod(name))\
    {\
        return  ClsType::thisMethodSet()->invokeMethod(this, name, args);\
    }\
    return PClsType::invokeMethodWithArgVec(name, args);\
}

#define  TW_CLASSMETHOD(ClsType, MethodName)\
{\
    typedef decltype(&ClsType::MethodName) MethodType;\
    ClsType::thisMethodSet()->addMethod(L#MethodName, new TwMetaMethod<MethodType>((MethodType)(&ClsType::MethodName)));\
}

#define  TW_PROPERTY(ClsType, Ty, name, setter, getter) \
TW_CLASSMETHOD(ClsType, setter)\
TW_CLASSMETHOD(ClsType, getter)\
{\
    ClsType::thisPropertySet()->addProperty(new TwProperty<ClsType, Ty>(name, ClsType::thisMethodSet()->findMethod(L#setter), ClsType::thisMethodSet()->findMethod(L#getter)));\
}

#define  TW_PROPERTY_READONLY(ClsType, Ty, name, getter) \
TW_CLASSMETHOD(ClsType, getter)\
{\
    ClsType::thisPropertySet()->addProperty(new TwProperty<ClsType, Ty>(name, nullptr, ClsType::thisMethodSet()->findMethod(L#getter)));\
}

#define  TW_PROPERTY_WRITEONLY(ClsType, Ty, name, setter) \
TW_CLASSMETHOD(ClsType, setter)\
{\
    ClsType::thisPropertySet()->addProperty(new TwProperty<ClsType, Ty>(name, ClsType::thisMethodSet()->findMethod(L#setter), nullptr));\
}

#define  TW_CLASSMEMEBER_BEGIN(ClsType, PClsType) \
TW_SUPPORTPROPERTY(ClsType, PClsType) \
TW_SUPPORTINVOKEMETHOD(ClsType, PClsType)\
virtual bool initInvokable()\
{\
    static bool hasInit = false;\
    if (hasInit)\
    {\
        return true;\
    }\
    __super::initInvokable();\
    hasInit = ClsType::twInitInvokable();\
    return hasInit;\
}\
static bool twInitInvokable()\
{\
    static bool hasInit = false;\
    if (hasInit)\
    {\
    return true;\
    }

#define  TW_CLASSMEMEBER_END\
    hasInit = true;\
    return true;\
}


// #define  TW_INIT_INVOKABLEMEMBER(ClsType) \
//     static const bool k_##ClsType##InvokableInit = ClsType::twInitInvokable();
// 
//////////////////////////////////////////////////////////////////////////

