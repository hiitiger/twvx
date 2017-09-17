#include "stable.h"

#include <algorithm>
#include "twobject.h"
#include "twpointer.h"
#include "twbaseservice.h"

TW_ROOTCLASS_IMPLEMENT(TwObject);

TwObject::TwObject( TwObject* parent /*= nullptr*/ )
: m_parent(parent)
, m_guardPtr(nullptr)
{
    m_guardPtr = new TwObjectGuard( this);
    if (parent)
    {
        parent->m_children.push_back(this);
    }
}

TwObject::~TwObject()
{
    if (m_guardPtr->hasPtr())
    {
        m_guardPtr->clearGuardObject( this);
    }
    delete m_guardPtr;
    m_guardPtr = nullptr;

    deleteChildren();
    setParent(nullptr);
}

void TwObject::deleteChildren()
{
    for (size_t i = 0; i < m_children.size(); ++i)
    {
        TwObject* child = m_children[i];
        m_children[i] = nullptr;
        child->m_parent = nullptr;
        delete child;
    }
    m_children.clear();
}


TwObjectGuard * TwObject::getGuard() const
{
    return m_guardPtr;
}

void TwObject::setParent( TwObject* parent )
{
    if (parent == this  || parent == m_parent)
    {
        return;
    }
    //romove from orignal parent
    if (m_parent)
    {
        std::vector<TwObject*>::iterator it = std::find(m_parent->m_children.begin(), m_parent->m_children.end(),this);
        if (it != m_parent->m_children.end())
        {
            m_parent->m_children.erase(it);
        }
        m_parent = nullptr;
    }
    //insert
    if (parent)
    {
        m_parent = parent;
        m_parent->m_children.push_back(this);
    }
}

TwObject* TwObject::parent() const
{
    return m_parent;
}

void TwObject::deleteLater()
{
    TwMessageLoop::current()->postFunction([weakSelf = TwWeakPtr<TwObject>(this)]() {
        if (weakSelf)
        {
            delete weakSelf;
        }
    });
}

