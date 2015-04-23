
#pragma once
#include "twcreator.h"


#define  TW_CLASS_DECLARE(ClsType)\
public:\
    static ClsType* create()\
    {\
        return new ClsType();\
    }\
    static TwObject* twCreate();\
    static TwClassInfo* thisClassInfo();\
    virtual TwClassInfo* classInfo() const;\
    virtual bool isType(TwClassInfo* i) const;\
    template<class Type> bool isType() const\
    {\
        return isType(Type::thisClassInfo());\
    }\
private:

#define  TW_CLASS_REGISTER(ClsType)\
    static const bool r_##ClsType = twRegisterClass(L#ClsType, ClsType::thisClassInfo());

#define  TW_CLASS_IMPLEMENT(ClsType, PClsType)\
    TwObject* ClsType::twCreate(){return ClsType::create();}\
    TwClassInfo* ClsType::thisClassInfo()\
    {\
        static TwClassInfo* this_ClassInfo = nullptr;\
        if (!this_ClassInfo)\
        {\
            this_ClassInfo = new TwClassInfo(L#ClsType, PClsType::thisClassInfo(), ClsType::twCreate);\
        }\
        return this_ClassInfo;\
    }\
    TwClassInfo* ClsType::classInfo() const{return ClsType::thisClassInfo();}\
    bool ClsType::isType(TwClassInfo* i) const\
    {\
        return ClsType::thisClassInfo() == i || PClsType::isType(i);\
    }

#define  TW_ROOTCLASS_IMPLEMENT(ClsType)\
    TwObject* ClsType::twCreate(){return ClsType::create();}\
    TwClassInfo* ClsType::thisClassInfo()\
    {\
        static TwClassInfo* this_ClassInfo = nullptr;\
        if (!this_ClassInfo)\
        {\
            this_ClassInfo = new TwClassInfo(L#ClsType, nullptr, ClsType::twCreate);\
        }\
        return this_ClassInfo;\
     }\
    TwClassInfo* ClsType::classInfo() const{return ClsType::thisClassInfo();}\
    bool ClsType::isType(TwClassInfo* i) const\
    {\
        return ClsType::thisClassInfo() == i; \
    }



//////////////////////////////////////////////////////////////////////////


#define  TW_CLASS_DYN_DECLARE(ClsType, OObjType, PClsType, pfnCreator)\
    TW_CLASS_DECLARE(ClsType,true)\
    static OObjType* staticCreator_##ClsType_##OObjType(PClsType* parent)\
    {\
        ClsType* newObj = new ClsType(parent);\
        return static_cast<OObjType*>(newObj);\
    }\
    static TwObjCCNodeHelper<OObjType, PClsType, pfnCreator> CCNodeHelper_##ClsType;

#define  TW_CLASS_DYN_IMPLEMENT(ClsType, OObjType, PClsType, pfnCreator)\
    TW_CLASS_IMPLEMENT(ClsType, PClsType)\
    TwObjCCNodeHelper<OObjType, PClsType, pfnCreator> ClsType::CCNodeHelper_##ClsType(new TwObjCCNode<pfnCreator>(L#ClsType,ClsType::staticCreator_##ClsType_##OObjType));


typedef TwObject* (*pfnObjDynCreator)(TwObject* parent);
#define  TW_OBJECT_DYN_DECLARE(ClsType)\
    TW_CLASS_DYN_DECLARE(ClsType, TwObject, TwObject, pfnObjDynCreator)

#define  TW_OBJECT_DYN_IMPLEMENT(ClsType)\
    TW_CLASS_DYN_IMPLEMENT(ClsType, TwObject, TwObject, pfnObjDynCreator)