#include "stable.h"

#include "twwindow.h"
#include "twpainter.h"
#include "twwidget.h"
#include "twlayout.h"
#include "tweventscrack.h"

#include "twrootwidget.h"

#define  TW_SYNC_OBJECTTREE_AND_WIDGETTREE 

//////////////////////////////////////////////////////////////////////////
TW_CLASS_IMPLEMENT(TwWidget, TwControl)

TwWidget::TwWidget( TwWidget* parent /*= nullptr*/ )
    : TwControl(parent)
    , sigVisibleChanged(this)
    , sigEnableChanged(this)
    , sigBoundChanged(this)
    , m_parentWidget(nullptr)
    , m_rootWidget(nullptr)
    , m_widthPolicy(Tw::Preferred)
    , m_heightPolicy(Tw::Preferred)
    , m_zOrder(0)
    , m_hAlignment(Tw::Align_Stretch)
    , m_vAlignment(Tw::Align_Stretch)
    , m_childWidgetSpacing(5)
    , m_contentMargin(5, 5, 5, 5)
    , m_layout(nullptr)
    , m_backGroundColor(0, 0, 0, 0)
    , m_visible(true)
    , m_enabled(true)
    , m_dirtyChildrenOrder(false)
    , m_acceptButtons(Tw::MB_LeftButton)
    , m_widgetFlags(0)
{
    setParentWidget(parent);
}

TwWidget::~TwWidget()
{
    //layouts
    if (m_parentWidget)
    {
        m_parentWidget->removeWidget(this);
    }
    m_layoutWidgets.clear();

    //widgets tree
    //!! this widget is destructed and so it's widget vector,
    //in object tree's delete processing , it's children will call removeWidget and get a destructed widget vector.
    for (auto i : m_childWidgets)
    {
        i->m_parentWidget = nullptr;
    }
    m_childWidgets.clear();

    setParentWidget(nullptr);
}

void TwWidget::setParentWidget(TwWidget* parentWidget)
{
#ifdef TW_SYNC_OBJECTTREE_AND_WIDGETTREE
    setParent(parentWidget);
#endif

    if (parentWidget == m_parentWidget || parentWidget == this)
    {
        return;
    }

    if (m_parentWidget)
    {
        m_parentWidget->removeChildWidget(this);
    }

    m_parentWidget = parentWidget;
    if (m_parentWidget)
    {
        m_parentWidget->addChildWidget(this);

        //TODO:
        setEnabled(m_parentWidget->isEnabled());

        update();
    }

}

TwWindow* TwWidget::window() const
{
    if (TwRootWidget* root = twObject_cast<TwRootWidget*>(rootWidget()))
    {
        return root->window();    
    }
    return nullptr;
}

void TwWidget::setRootWidget( TwWidget* root )
{
    if (m_rootWidget != root)
    {
        m_rootWidget = root;

        for (auto w : m_childWidgets)
        {
            w->setRootWidget(m_rootWidget);
        }
    }
}

bool TwWidget::isAncestorOf(const TwWidget* w)
{
    for (const TwWidget* wparent = w; wparent != nullptr; wparent = w->parentWidget())
    {
        if (wparent == this)
        {
            return true;
        }
    }
    return false;
}

TwWidget* TwWidget::findChildWidget(const std::wstring& name)
{
    for (auto w : m_childWidgets)
    {
        if (w->name() == name)
        {
            return w;
        }
    }

    TwWidget* widget = nullptr;
    for (auto w: m_childWidgets)
    {
        widget = w->findChildWidget(name);
        if (widget)
        {
            return widget;
        }
    }

    return nullptr;
}

void TwWidget::addWidget(TwWidget* w)
{
    auto it = std::find(m_layoutWidgets.begin(), m_layoutWidgets.end(), w);
    if (it != m_layoutWidgets.end())
    {
        return;
    }

    w->setParentWidget(this);

    m_layoutWidgets.push_back(w);
    onWidgetAdded(w);
}

void TwWidget::removeWidget(TwWidget* w)
{
    auto it = std::find(m_layoutWidgets.begin(), m_layoutWidgets.end(), w);
    if (it == m_layoutWidgets.end())
    {
        return;
    }
    else
    {
        m_layoutWidgets.erase(it);
        onWidgetRemoved(w);
    }
}

int TwWidget::widgetCount() const
{
    return m_layoutWidgets.size();
}

TwWidget* TwWidget::widgetAt(int index)
{
    DAssert(index < childWidgetCount() && index >= 0);
    if (index < childWidgetCount() && index >= 0)
    {
        return m_layoutWidgets[index];
    }
    return nullptr;
}

bool TwWidget::isWidgetAdded(TwWidget* w)
{
    auto it = std::find(m_layoutWidgets.begin(), m_layoutWidgets.end(), w);
    return it != m_layoutWidgets.end();
}

void TwWidget::addChildWidget( TwWidget* w )
{
    std::vector<TwWidget*>::iterator it = std::find(m_childWidgets.begin(), m_childWidgets.end(), w);
    if (it != m_childWidgets.end())
    {
        return;
    }

    if (m_childWidgets.size() == 0)
    {
        setWidgetFlag(Tw::HasChildren, true);
    }

    w->setRootWidget(this->rootWidget());

    m_childWidgets.push_back(w);

    markDirtyChildrenOrder();
}

void TwWidget::removeChildWidget( TwWidget* w )
{
    std::vector<TwWidget*>::iterator it = std::find(m_childWidgets.begin(), m_childWidgets.end(), w);
    if ( it == m_childWidgets.end())
    {
        return;
    }

    w->setRootWidget(nullptr);

    m_childWidgets.erase(it);

    if (m_childWidgets.size() == 0)
    {
        setWidgetFlag(Tw::HasChildren, false);
    }
}

int TwWidget::childWidgetCount() const
{
    return m_childWidgets.size();
}

TwWidget* TwWidget::childWidgetAt( int index )
{
    DAssert(index < childWidgetCount() && index >= 0); 
    if (index < childWidgetCount() && index >= 0)
    {
        return m_childWidgets[index];
    }
    return nullptr;
}

void TwWidget::syncAllChildrenOrder()
{
    for (auto w : m_childWidgets)
    {
        if (w->testWidgetFlag(Tw::HasChildren))
        {
            w->sortChildren();
            w->syncAllChildrenOrder();
        }
    }
}

void TwWidget::sortChildren()
{
    if (m_dirtyChildrenOrder)
    {
        if (childWidgetCount() == 0)
        {
            return;
        }
        //sort
        std::stable_sort(m_childWidgets.begin(), m_childWidgets.end(), [](TwWidget* w1, TwWidget* w2)
        {
            return w1->zOrder() < w2->zOrder();
        });

        m_dirtyChildrenOrder = false;
    }
}

void TwWidget::markDirtyChildrenOrder()
{
    m_dirtyChildrenOrder = true;
}

void TwWidget::markDirtyLayout()
{
    if (m_layout)
    {
        m_layout->invalidateLayout();
    }
}

void TwWidget::markDirtyRect()
{
    if (TwRootWidget* root = twObject_cast<TwRootWidget*>(rootWidget()))
    {
        return root->schedulePaintWidget(this);
    }
}

TwWidget* TwWidget::eventHandlerFromPoint( const TwPoint<int>& pt )
{
    if (isVisible() && contains(pt))
    {
        for (int i = childWidgetCount()-1; i >= 0; --i)
        {
            TwWidget* widget = childWidgetAt(i);
           
            TwPoint<int> wpt = widget->mapFromParent(pt);
            
            widget = widget->eventHandlerFromPoint(wpt);
            if (widget != nullptr)
            {
                return widget;
            }
        }
        return this;
    }
    return nullptr;
}

void TwWidget::show()
{
    setVisible(true);
}

void TwWidget::hide()
{
    setVisible(false);
}

void TwWidget::setVisible( bool visible )
{
    if (m_visible != visible)
    {
        m_visible = visible;

        onVisibleChanged();
        if (parentWidget())
        {
            parentWidget()->onChildWidgetChange(this, TwWidget::VisibleChange);
        }

        sigVisibleChanged.emit();
    }
}

void TwWidget::setEnabled(bool enable)
{
    if (m_enabled != enable)
    {
        m_enabled = enable;

        if (!m_enabled)
        {
            clearFocus();
        }
        //TODO:
        for (auto w : m_childWidgets)
        {
            w->setEnabled(m_enabled);
        }

        onEnableChanged();

        sigEnableChanged.emit();
    }
}

void TwWidget::updateBound()
{
    TwSize<int> sz = this->calcLayoutSize();

    if (size() != sz)
    {
        if (parentWidget())
        {
            parentWidget()->onChildWidgetChange(this, TwWidget::CalcedLayoutSizeChange);
        }
    }
}

void TwWidget::update()
{
    markDirtyRect();
}

void TwWidget::setFocus()
{
    if (TwRootWidget* root = twObject_cast<TwRootWidget*>(rootWidget()))
    {
        root->setFocosWidget(this);
    }
}

void TwWidget::clearFocus()
{
    if (hasFocus())
    {
        TwRootWidget* root = twObject_cast<TwRootWidget*>(rootWidget());
        root->setFocosWidget(nullptr);
    }
}

bool TwWidget::hasFocus() const
{
    if (TwRootWidget* root = twObject_cast<TwRootWidget*>(rootWidget()))
    {
        return this == root->focusWidget();
    }
    else
    {
        return false;
    }
}

void TwWidget::setAcceptMouseButton(Tw::MouseButton button, bool accept)
{
    if (accept)
    {
        m_acceptButtons |= static_cast<int>(button);
    }
    else
    {
        m_acceptButtons &= static_cast<int>(~button);
    }
}

bool TwWidget::acceptMouseButton(Tw::MouseButton button)
{
    return (m_acceptButtons & button) != 0;
}

void TwWidget::setWidgetFlag(Tw::WidgetFlag flag, bool set)
{
    if (set)
    {
        m_widgetFlags |= static_cast<int64>(flag);
    }
    else
    {
        m_widgetFlags &= static_cast<int64>(~flag);
    }
}

void TwWidget::setPos( const TwPoint<int>& pt )
{
    setBoundRect(TwRect<int>(pt, size()));
}

void TwWidget::setPos( int x, int y )
{
    setPos(TwPoint<int>(x ,y));
}

TwPoint<int> TwWidget::pos() const
{
    return m_boundRect.topLeft();
}

void TwWidget::setX( int x )
{
    setBoundRect(x, y(), width(), height());
}

int TwWidget::x() const
{
    return m_boundRect.left();
}

void TwWidget::setY( int y )
{
    setBoundRect(x(), y, width(), height());
}

int TwWidget::y() const
{
    return m_boundRect.top();
}

void TwWidget::setSize( const TwSize<int>& sz )
{
    setBoundRect(TwRect<int>(pos(), sz));
    setSizePolicy(Tw::Fixed, Tw::Fixed);
}

void TwWidget::setSize( int width, int height )
{
    setSize(TwSize<int>(width, height));
}

TwSize<int> TwWidget::size() const
{
    return m_boundRect.size();
}

int TwWidget::width() const
{
    return m_boundRect.width();
}

void TwWidget::setWidth( int w )
{
    setBoundRect(x(), y(), w, height());
    setSizePolicy(Tw::Fixed, m_heightPolicy);
}

int TwWidget::height() const
{
    return m_boundRect.height();
}

void TwWidget::setHeight( int h )
{
    setBoundRect(x(), y(), width(), h);
    setSizePolicy(m_widthPolicy, Tw::Fixed);
}

void TwWidget::setSizePolicy(Tw::SizePolicy widthPolicy, Tw::SizePolicy heightPolicy)
{
    if (m_widthPolicy != widthPolicy || m_heightPolicy != heightPolicy)
    {
        m_widthPolicy = widthPolicy;
        m_heightPolicy = heightPolicy;
    }
}

void TwWidget::setBoundRect( const TwRect<int>& rc )
{
    if (m_boundRect != rc)
    {
        TwRect<int> oldBound = m_boundRect;
        m_boundRect = rc;

        onBoundChanged(oldBound);

        sigBoundChanged.emit();
    }
}

void TwWidget::setBoundRect(const TwPoint<int>& topLeft, const TwSize<int>& sz)
{
    setBoundRect(TwRect<int>(topLeft, sz));
}

void TwWidget::setBoundRect( int left, int top, int width, int height )
{
    setBoundRect(TwRect<int>(left, top, left+width, top+height));
}

TwRect<int> TwWidget::boundRect() const
{
    return m_boundRect;
}

TwRect<int> TwWidget::localRect() const
{
    return boundRect().movedTo(0,0);
}

bool TwWidget::contains(const TwPoint<int>& localPoint)
{
    return localRect().contains(localPoint);
}

TwRect<int> TwWidget::rectInWindow() const
{
    return mapToWindow(localRect());
}

TwRect<int> TwWidget::rectInRoot() const
{
    return mapRect(rootWidget(), this, localRect());
}

void TwWidget::setZOrder(int z)
{
    if (m_zOrder != z)
    {
        m_zOrder = z;

        if (parentWidget())
        {
            parentWidget()->onChildWidgetChange(this, TwWidget::ZOrderChange);
        }
    }
}

void TwWidget::setLayout( LayoutType layoutType )
{
    TwLayoutManager* newLayout = nullptr;
    switch (layoutType)
    {
    case TwWidget::Default:
        newLayout = nullptr;
        break;
    case TwWidget::HBox:
        newLayout = new TwBoxLayout(this, Tw::Horizontal);
        break;
    case TwWidget::VBox:
        newLayout = new TwBoxLayout(this, Tw::Vertical);
        break;
    case TwWidget::Anchor:
        newLayout = new TwAnchorLayout(this);
        break; 
    case TwWidget::Fill:
        newLayout = new TwFillLayout(this);
        break;
    default:
        break;
    }
    DAssert_M(newLayout, "layout is empty");
    setLayout(newLayout);
}

void TwWidget::setLayout( TwLayoutManager* layout)
{
    m_layout.reset(layout);
    if (m_layout)
    {
        m_layout->setHostWidget(this);
    }
}

void TwWidget::setContentMargin( const TwMargin<int>& margin )
{
    m_contentMargin = margin;
    //TODO
    markDirtyLayout();
}

TwMargin<int> TwWidget::contentMargin() const
{
    return m_contentMargin;
}

void TwWidget::setChildSpacing( int space )
{
    m_childWidgetSpacing = space;
    markDirtyLayout();
}

int TwWidget::childSpacing() const
{
    return m_childWidgetSpacing;
}

void TwWidget::setAlignment(Tw::Alignment hAlign, Tw::Alignment vAlign)
{
    setHorizontalAlignment(hAlign);
    setVerticalAlignment(vAlign);
}

void TwWidget::setHorizontalAlignment(Tw::Alignment hAlign)
{
    if (m_hAlignment != hAlign)
    {
        m_hAlignment = hAlign;

        if (parentWidget())
        {
            parentWidget()->onChildWidgetChange(this, TwWidget::AlignmentChange);
        }
    }
}

void TwWidget::setVerticalAlignment(Tw::Alignment vAlign)
{
    if (m_vAlignment != vAlign)
    {
        m_vAlignment = vAlign;

        if (parentWidget())
        {
            parentWidget()->onChildWidgetChange(this, TwWidget::AlignmentChange);
        }
    }
}

void TwWidget::layout()
{
    layoutInternalWidgets();
    layoutWidgets();
}

void TwWidget::layoutInternalWidgets()
{

}

void TwWidget::layoutWidgets()
{
    if (m_layout)
    {
        m_layout->layout();
    }
}


TwSize<int> TwWidget::calcLayoutSize() const
{
    if (m_widthPolicy == Tw::Fixed && m_heightPolicy == Tw::Fixed)
    {
        return size();
    }
    else
    {
        TwSize<int> sz = preferredSize();
        if (m_widthPolicy == Tw::Fixed)
        {
            sz.setWidth(width());
        }
        if (m_heightPolicy == Tw::Fixed)
        {
            sz.setHeight(height());
        }
        return sz;
    }
}


TwSize<int> TwWidget::preferredSize() const
{
    TwSize<int> result;

    if (m_layout)
    {
        result = m_layout->preferredSize();
    }

    result.setWidth(twMax(result.width(), hintSize().width()));
    result.setHeight(twMax(result.height(), hintSize().height()));

    return result;
}

TwSize<int> TwWidget::hintSize() const
{
    return TwSize<int>();
}

void TwWidget::setCursor(TwCursor cursor)
{
    m_cursor.reset(new TwCursor(cursor));
}

void TwWidget::clearCursor()
{
    m_cursor.reset();
}

TwCursor TwWidget::cursor() const
{
    if (m_cursor)
    {
        return *(m_cursor.get());
    }
    else if (parentWidget())
    {
        return parentWidget()->cursor();
    }
    return TwCursor();
}

//////////////////////////////////////////////////////////////////////////
TwPoint<int> TwWidget::mapToParent( const TwPoint<int>& pt ) const
{
    //TODO: use transform
    return pt + pos();
}

TwPoint<int> TwWidget::mapToRoot( const TwPoint<int>& pt ) const
{
    //TODO
    if (parentWidget())
    {
        return   parentWidget()->mapToRoot(mapToParent(pt));
    }

    return pt + pos();
}

TwPoint<int> TwWidget::mapFromParent( const TwPoint<int>& pt ) const
{
    //TODO
    return pt - pos();
}

TwPoint<int> TwWidget::mapFromRoot( const TwPoint<int>& pt ) const
{
    //TODO
    TwPoint<int> offset = mapToRoot(TwPoint<int>(0, 0));
    return pt - offset;
}

TwRect<int> TwWidget::mapToParent( const TwRect<int>& rc ) const
{
    //TODO
    return TwRect<int>(mapToParent(rc.topLeft()), rc.size());
}

TwRect<int> TwWidget::mapToRoot( const TwRect<int>& rc ) const
{
    //TODO
    return TwRect<int>(mapToRoot(rc.topLeft()), rc.size());
}


TwRect<int> TwWidget::mapFromParent( const TwRect<int>& rc ) const
{
    //TODO
    return TwRect<int>(mapFromParent(rc.topLeft()), rc.size());
}

TwRect<int> TwWidget::mapFromRoot( const TwRect<int>& rc ) const
{
    //TODO
    return TwRect<int>(mapFromParent(rc.topLeft()), rc.size());
}

TwPoint<int> TwWidget::mapPoint( const TwWidget* dst, const TwWidget* src, const TwPoint<int>& ptInSrc )
{
    //TODO
    if (dst->window() == src->window())
    {
        return dst->mapFromWindow(src->mapToWindow(ptInSrc));
    }
    else
    {
        //TODO:

        TwPoint<int> ptInDst = ptInSrc;
        return ptInDst;
    }
}

TwRect<int> TwWidget::mapRect( const TwWidget* dst, const TwWidget* src, const TwRect<int>& rcInSrc )
{
    TwPoint<int> newPos = mapPoint(dst, src, rcInSrc.topLeft());
    return TwRect<int>(newPos, rcInSrc.size());
}

TwPoint<int> TwWidget::mapToWindow(const TwPoint<int>& pt ) const
{
    DAssert(window());
    TwPoint<int> pointInRoot = mapToRoot(pt);
    return window()->mapFromClientToWindow(pointInRoot);
}

TwPoint<int> TwWidget::mapFromWindow(const TwPoint<int>& pt ) const
{
    DAssert(window());
    TwPoint<int> pointInRoot = window()->mapFromWindowToClient(pt);
    return pointInRoot = mapFromRoot(pointInRoot);
}

TwRect<int> TwWidget::mapToWindow(const TwRect<int>& rc ) const
{
    return TwRect<int>(mapToWindow( rc.topLeft()), rc.size());
}

TwRect<int> TwWidget::mapFromWindow( const TwRect<int>& rc ) const
{
    return TwRect<int>(mapFromWindow(rc.topLeft()), rc.size());
}

void TwWidget::setBackgroundColor(const TwColor& color)
{
    m_backGroundColor = color;
}

void TwWidget::paintWidgetAndChildren(TwPainter* painter)
{
    paintBackground(painter);
    paint(painter);
    paintChildren(painter);
}

void TwWidget::paintBackground( TwPainter* painter)
{
    //TODO
    if (m_backGroundColor.alpha())
    {
        painter->fillRect(localRect(), m_backGroundColor);
    }
}

void TwWidget::paint( TwPainter* painter )
{
    TW_UNUSED(painter);
}

void TwWidget::paintChildren( TwPainter* painter )
{
    sortChildren();

    //get real clipped paint rect
    TwRect<int> paintRect = this->localRect();
    TwRect<int> clipRect = painter->lastPushedClipRect();
    if (clipRect.isValid())
    {
        paintRect.intersectWith(clipRect);
    }

    for (int i = 0; i != childWidgetCount(); ++i)
    {
        TwWidget* widget = childWidgetAt(i);
        if (widget->isVisible())
        {
            TwRect<int> childBound = widget->boundRect();
            TwRect<int> childClipRect = paintRect.intersectedWith(childBound);

            if (childClipRect.isValid())
            {
                painter->saveState();
                painter->translate(widget->pos());
                painter->pushClipRect(widget->mapFromParent(childClipRect));

                widget->paintWidgetAndChildren(painter);

                painter->restoreState();
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////
void TwWidget::onVisibleChanged()
{

}

void TwWidget::onEnableChanged()
{
    update();
}

void TwWidget::onBoundChanged( const TwRect<int>& oldRect )
{
    if (oldRect.size() != size())
    {
        layout();

        TwResizeEvent ev(size(), oldRect.size());
        TwEventDispatcher::sendEvent(this, &ev);
    }

    update();
}

void TwWidget::onChildWidgetChange(TwWidget* w, TwWidget::ChangeType changeType)
{
    switch (changeType)
    {
    case TwWidget::VisibleChange:
        {
            markDirtyLayout();
        }
        break;
    case TwWidget::EnableChange:
        break;
    case TwWidget::CalcedLayoutSizeChange:
        {
            if (isWidgetAdded(w) && m_layout)
            {
                markDirtyLayout();
            }
            else
            {
                //TODO:
                w->setBoundRect(TwPoint<int>(x(), y()), w->calcLayoutSize());
            }
        }
        break;
    case TwWidget::ZOrderChange:
        {
            markDirtyChildrenOrder();
            markDirtyLayout();
        }
        break;
    case TwWidget::AlignmentChange:
        break;
    default:
        break;
    }
}

void TwWidget::onWidgetAdded( TwWidget* w )
{
    markDirtyLayout();
    if (m_layout)
    {
        m_layout->onWidgetAdd(w);
    }
}

void TwWidget::onWidgetRemoved( TwWidget* w )
{
    markDirtyLayout();
    if (m_layout)
    {
        m_layout->onWidgetRemove(w);
    }
}

void TwWidget::onEvent( TwEvent* ev)
{
    if (!isEnabled())
    {
        switch (ev->id())
        {
        case Tw::Ev_MousePress:
        case Tw::Ev_MouseRelease:
        case Tw::Ev_MouseDbClick:
        case Tw::Ev_MouseMove:
        case Tw::Ev_HoverMove:
        case Tw::Ev_HoverEnter:
        case Tw::Ev_HoverLeave:
        case Tw::Ev_KeyPress:
        case Tw::Ev_KeyRelease:
        case Tw::Ev_Char:
        case Tw::Ev_FocusIn:
        {
            ev->reject();
            return;
        }
        default:
            break;
        }
    }
    TW_EVENTCRACK_BEGIN(ev)
        TW_EVENTSINK_R(ev, TwResizeEvent, Tw::Ev_Resize, this, onResizeEvent)

        TW_EVENTSINK_R(ev, TwMouseEvent, Tw::Ev_MousePress, this, onMousePresseEvent)
        TW_EVENTSINK_R(ev, TwMouseEvent, Tw::Ev_MouseRelease, this, onMouseReleaseEvent)
        TW_EVENTSINK_R(ev, TwMouseEvent, Tw::Ev_MouseDbClick, this, onMouseDBClicked)
        TW_EVENTSINK_R(ev, TwMouseEvent, Tw::Ev_MouseMove, this, onMouseMoveEvent)

        TW_EVENTSINK_R(ev, TwHoverEvent, Tw::Ev_HoverMove, this, onHoverMoveEvent)
        TW_EVENTSINK_R(ev, TwHoverEvent, Tw::Ev_HoverEnter, this, onHoverEnterEvent)
        TW_EVENTSINK_R(ev, TwHoverEvent, Tw::Ev_HoverLeave, this, onHoverLeaveEvent)

        TW_EVENTSINK_R(ev, TwWheelEvent, Tw::Ev_Wheel, this, onWheelEvent)

        TW_EVENTSINK_R(ev, TwKeyEvent, Tw::Ev_KeyPress, this, onKeyPresseEvent)
        TW_EVENTSINK_R(ev, TwKeyEvent, Tw::Ev_KeyRelease, this, onKeyReleaseEvent)
        TW_EVENTSINK_R(ev, TwCharEvent, Tw::Ev_Char, this, onCharEvent)

        TW_EVENTSINK_R(ev, TwFocusEvent, Tw::Ev_FocusIn, this, onFocusInEvent)
        TW_EVENTSINK_R(ev, TwFocusEvent, Tw::Ev_FocusOut, this, onFocusInEvent)

        TW_EVENTSINK_R(ev, TwEvent, Tw::Ev_LayoutRequest, this, onLayoutRequestEvent)

    TW_EVENTCRACK_END()

}

void TwWidget::onResizeEvent( TwResizeEvent* ev )
{
    TW_UNUSED(ev);
}

void TwWidget::onMousePresseEvent( TwMouseEvent* ev )
{
    TW_UNUSED(ev);
}

void TwWidget::onMouseReleaseEvent( TwMouseEvent* ev )
{
    TW_UNUSED(ev);
}

void TwWidget::onMouseDBClicked( TwMouseEvent* ev )
{
    TW_UNUSED(ev);
}

void TwWidget::onMouseMoveEvent(TwMouseEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWidget::onHoverMoveEvent( TwHoverEvent* ev )
{
    TW_UNUSED(ev);
}

void TwWidget::onHoverEnterEvent( TwHoverEvent* ev )
{
    TW_UNUSED(ev);
}

void TwWidget::onHoverLeaveEvent( TwHoverEvent* ev )
{
    TW_UNUSED(ev);
}

void TwWidget::onWheelEvent( TwWheelEvent* ev )
{
    TW_UNUSED(ev);
}

void TwWidget::onKeyPresseEvent( TwKeyEvent* ev )
{
    TW_UNUSED(ev);
}

void TwWidget::onKeyReleaseEvent( TwKeyEvent* ev )
{
    TW_UNUSED(ev);
}

void TwWidget::onCharEvent(TwCharEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWidget::onFocusInEvent(TwFocusEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWidget::onFocusOutEvent(TwFocusEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWidget::onLayoutRequestEvent(TwEvent* ev)
{
    TW_UNUSED(ev);
    if (m_layout)
    {
        m_layout->activeLayout();
    }
    updateBound();
    update();
}
