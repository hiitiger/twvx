#include "stable.h"
#include "twwidget.h"

#include "twlayout.h"


TwLayoutManager::TwLayoutManager(TwObject* parent)
    : TwObject(parent)
    , m_hostWidget(nullptr)
    , m_dirty(false)
{

}

TwLayoutManager::~TwLayoutManager()
{

}

void TwLayoutManager::setHostWidget( TwWidget* w)
{
    if (m_hostWidget == w)
    {
        return;
    }

    if (m_hostWidget)
    {
        m_hostWidget->setLayout(nullptr);
    }

    m_hostWidget = w;
    m_dirty = false;
    invalidateLayout();
}

void TwLayoutManager::layout()
{
    if (m_dirty || m_cacheHostSize != m_hostWidget->size())
    {
        setupGeometry();
        layoutWidgets();
        m_cacheHostSize = m_hostWidget->size();
    }
}

void TwLayoutManager::activeLayout()
{
    m_dirty = true;
    layout();
}

void TwLayoutManager::invalidateLayout()
{
    if (!m_dirty)
    {
        m_dirty = true;
        TwEventDispatcher::postEvent(m_hostWidget, new TwEvent(Tw::Ev_LayoutRequest));
    }
}

void TwLayoutManager::onWidgetAdd( TwWidget* child )
{
    TW_UNUSED(child);
}

void TwLayoutManager::onWidgetRemove( TwWidget* child )
{
    TW_UNUSED(child);
}

TwSize<int> TwLayoutManager::preferredSize() 
{
    setupGeometry();
    return m_cachePerredSize;
}

void TwLayoutManager::setupGeometry()
{
    if (m_dirty)
    {
        m_cachePerredSize = m_hostWidget->size();
        m_dirty = false;
    }
}
