#include "stable.h"
#include "twobject.h"
#include "twpointer.h"



TwObject::TwObject( TwObject* parent /*= NULL*/ )
: m_parent(parent)
, _sObjHelper(0)
{
    _sObjHelper = new sObjHelper( this);
}

TwObject::~TwObject()
{
    if (_sObjHelper->hasPtr())
    {
        _sObjHelper->clearPtr( this);
    }
}

sObjHelper * TwObject::getHelper() const
{
    return _sObjHelper;
}

void TwObject::setParent( TwObject* parent )
{
    if (parent == this  || parent == m_parent)
    {
        return;
    }
    //TXXTODO:
    //romove from orignal parent

    //insert
}

TwObject* TwObject::parent() const
{
    return m_parent;
}

