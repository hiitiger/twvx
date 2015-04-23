#pragma once

struct TwClassInfo;

//////////////////////////////////////////////////////////////////////////
class TW_BASE_API TwCreator
{

public:
    static TwCreator* twCreator();
    
    bool registerClass(const std::wstring& classname,TwClassInfo* o);
    bool exist(const std::wstring& classname);

    TwObject* createObject(const std::wstring& clsname);

private:
    TwCreator();
    ~TwCreator();

    std::map<std::wstring, std::shared_ptr<TwClassInfo> > m_classMap;
    TwLock m_classMapLock;
};

TW_BASE_API TwCreator* twCreator();

TW_BASE_API bool twRegisterClass(const std::wstring& cls, TwClassInfo* o);
//////////////////////////////////////////////////////////////////////////
/*
classname may not match ObjType,so need a node to hold it;
*/

template< class CType>
struct TwObjCCNode
{
    typedef CType creatorFnType;

    TwObjCCNode(const wchar_t* clsname, creatorFnType fn)
    :name(clsname), creator(fn)
    {;}
    std::wstring name;
    creatorFnType creator;
};


template <class TType, class PType, class CType>
struct TwObjCCNodeList
{
    typedef TType ObjType;
    typedef PType ParentType;
    typedef CType creatorFnType;

    static void addCCNode(TwObjCCNode<creatorFnType>* node)
    {
        getCCMap().insert(std::make_pair(node->name,node));
    }

    static void removeCCNode(TwObjCCNode<creatorFnType>* node)
    {
        getCCMap().erase(node->name);
    }

    static ObjType* createObject(std::wstring classname,ParentType* parent)
    {
        std::map<std::wstring, TwObjCCNode<creatorFnType>*>::iterator it = getCCMap().find(classname);
        if (it == getCCMap().end())
        {
            return nullptr;
        }
        return it->second->creator(parent);
    }

    static std::map<std::wstring, TwObjCCNode<creatorFnType>*>& getCCMap()
    {
        static std::map<std::wstring, TwObjCCNode<creatorFnType>*> ccmap;
        return ccmap;
    }
};

template <class TType, class PType, class CType>
struct TwObjCCNodeHelper
{
    typedef TType ObjType;
    typedef PType ParentType;
    typedef CType creatorFnType;

    TwObjCCNodeHelper(TwObjCCNode<creatorFnType>* n)
    :node(n)
    {
        TwObjCCNodeList<ObjType,ParentType,creatorFnType>::addCCNode(n);
    }
    ~TwObjCCNodeHelper()
    {
        TwObjCCNodeList<ObjType,ParentType,creatorFnType>::removeCCNode(node);
        delete node;
    }

private:
    TwObjCCNode<creatorFnType>* node;
};

//////////////////////////////////////////////////////////////////////////