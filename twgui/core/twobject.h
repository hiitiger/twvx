#pragma once
#include <list>
/*
TwObject is the base object type of all framework objects.
It supports:
  * guarded pointer
  * delete later
  * runtime property
*/


class sObjHelper;

class TW_GUI_API TwObject 
{
public:
    explicit TwObject(TwObject* parent = NULL);
    virtual ~TwObject();

    void setParent(TwObject* parent);
    TwObject* parent() const;

    void deleteLater();

public:
    sObjHelper *getHelper() const;

private:
    void _delChildren();

private:
    TwObject* m_parent;
    sObjHelper *_sObjHelper;
    std::list<TwObject*> m_children;
};