#include "stable.h"

#include "tweventscrack.h"
#include "twwindow.h"
#include "twwindowevendispatch.h"
#include "twrootwidget.h"


class DefaultContentWidget : public TwWidget
{
public:
    DefaultContentWidget(TwWidget* parent = nullptr)
        :TwWidget(parent)
    {
        setLayout(TwWidget::VBox);
    }
};


TwRootWidget::TwRootWidget(TwWindow* window /*= nullptr*/)
    : TwWidget(nullptr)
    , m_twWindow(window)
    , m_contentWidget(nullptr)
    , m_mousePressHandler(nullptr)
    , m_focusWidget(nullptr)
{
    m_rootWidget = this;
    setName(L"RootWidget");
    setBackgroundColor(TwColor(209, 212, 219));
    setLayout(TwWidget::Fill);
    setContentWidget(new DefaultContentWidget(nullptr));
}

TwRootWidget::~TwRootWidget()
{
    delete m_contentWidget;
}

TwWindow* TwRootWidget::window() const
{
    return m_twWindow;
}

void TwRootWidget::setContentWidget(TwWidget* contentWidget, bool deleteOld /*= true*/)
{
    if (contentWidget == m_contentWidget)
    {
        return;
    }
    if (deleteOld)
    {
        delete m_contentWidget;
    }
    m_contentWidget = contentWidget;
    m_contentWidget->setName(L"ContentWidget");
    addWidget(m_contentWidget);
}

TwWidget* TwRootWidget::contentWidget() const
{
    return m_contentWidget;
}

void TwRootWidget::updateBound()
{
    m_twWindow->updateWindowRect();
}

void TwRootWidget::schedulePaintWidget(TwWidget*)
{
    m_twWindow->eventDispatcher()->schedulePaint();
}

TwWidget* TwRootWidget::resetWidgetUnderMouse(const TwPoint<int>& pt)
{
    m_widgetUnderMouse = eventHandlerFromPoint(pt);
    return m_contentWidget;
}

TwWidget* TwRootWidget::pickKeyEventReciever()
{
    //TODO:
    return this;
}

TwWidget* TwRootWidget::pickFocusWidget()
{
    return m_focusWidget ? m_focusWidget.data() : this;
}

void TwRootWidget::setFocosWidget(TwWidget* w)
{
    if (m_focusWidget != w)
    {
        if (m_focusWidget)
        {
            TwFocusEvent focusOutEvent(Tw::Ev_FocusOut);
            TwEventDispatcher::sendEvent(m_focusWidget, &focusOutEvent);
        }

        m_focusWidget = nullptr;
        if (w)
        {
            TwFocusEvent focusInEvent(Tw::Ev_FocusIn);
            TwEventDispatcher::sendEvent(w, &focusInEvent);
            if (focusInEvent.accepted())
            {
                m_focusWidget = w;
            }
        }
    }
}

TwWidget* TwRootWidget::focusWidget() const
{
    return m_focusWidget;
}


void TwRootWidget::dispatchHitTestEvent(TwHitTestEvent* ev)
{
    TwWidget* widgetUnderMouse = eventHandlerFromPoint(ev->localPos());
    if (widgetUnderMouse == this || widgetUnderMouse == nullptr)
    {
        ev->reject();
    }
    else
    {

    }
}

void TwRootWidget::dispatchMouseEvent(TwMouseEvent* ev)
{
    TwPoint<int> rootPos = ev->localPos();

    if (ev->id() == Tw::Ev_MouseMove)
    {
        if (m_mousePressHandler && m_mousePressHandler->isEnabled())
        {
            ev->m_localPos = m_mousePressHandler->mapFromRoot(rootPos);
            m_mousePressHandler->processEvent(ev);
        }
        return;
    }

    resetWidgetUnderMouse(rootPos);

    if (ev->id() == Tw::Ev_MousePress || ev->id() == Tw::Ev_MouseDbClick)
    {
        for (TwWidget* r = m_widgetUnderMouse; r && r != this; r = r->parentWidget())
        {
            //TODO: check if r is able to handle events now
            if (!r->isEnabled())
            {
                m_mousePressHandler = nullptr;
                return;
            }
            else if (r->acceptMouseButton(ev->button()))
            {
                ev->m_localPos = r->mapFromRoot(rootPos);
                r->processEvent(ev);
                if (ev->accepted())
                {
                    m_mousePressHandler = r;
                    return;
                }
            }
        }
        m_mousePressHandler = nullptr;
    }
    else if (ev->id() == Tw::Ev_MouseRelease)
    {
        if (m_mousePressHandler)
        {
            ev->m_localPos = m_mousePressHandler->mapFromRoot(rootPos);

            m_mousePressHandler->processEvent(ev);
        }

        m_mousePressHandler = nullptr;
    }
}

void TwRootWidget::disPatchHoverEvent(TwHoverEvent* ev)
{
    resetWidgetUnderMouse(ev->localPos());

    if (ev->id() == Tw::Ev_HoverMove)
    {
        //handle mouse move should be careful
        std::list<TwWidget*> hoverWidgets;
        {
            TwWidget* w = m_widgetUnderMouse;
            while (w)
            {
                hoverWidgets.push_front(w);
                w = w->parentWidget();
            }
        }

        std::list<TwWidget*> news;
        std::list<TwWidget*> erases;

        checkWidgets(m_hoverHandlers, hoverWidgets, news, erases);

        for (auto w : erases)
        {
            if (w->isEnabled() && w->acceptHoverEvent())
            {
                TwHoverEvent hoverLeaveEv(Tw::Ev_HoverLeave, ev->screenPos(), ev->clientPos(), mapPoint(w, this, ev->localPos()));
                w->processEvent(&hoverLeaveEv);
            }
        }

        for (auto w : news)
        {
            if (w->isEnabled() && w->acceptHoverEvent())
            {
                TwHoverEvent hoverEnterEv(Tw::Ev_HoverEnter, ev->screenPos(), ev->clientPos(), mapPoint(w, this, ev->localPos()));
                w->processEvent(&hoverEnterEv);
            }
        }

        for (auto w : m_hoverHandlers)
        {
            if (w->isEnabled() && w->acceptHoverEvent())
            {
                TwHoverEvent hoverMoveEv(Tw::Ev_HoverMove, ev->screenPos(), ev->clientPos(), w->mapFromRoot(ev->localPos()));
                w->processEvent(&hoverMoveEv);
            }
            break;
        }

        for (auto w : news)
        {
            m_hoverHandlers.push_back(w);
        }

    }
    else if (ev->id() == Tw::Ev_HoverLeave)
    {
        for (auto w : m_hoverHandlers)
        {
            if (w && w->isEnabled() && w->acceptHoverEvent())
            {
                TwHoverEvent hoverLeaveEv(Tw::Ev_HoverLeave, ev->screenPos(), ev->clientPos(), w->mapFromRoot(ev->localPos()));
                w->processEvent(&hoverLeaveEv);
            }
        }
        m_hoverHandlers.clear();
    }
}

void TwRootWidget::disPatachWheelEvent(TwWheelEvent* ev)
{
    if (m_mousePressHandler)
    {
        m_mousePressHandler->processEvent(ev);
    }
    else
    {
        resetWidgetUnderMouse(ev->localPos());
        for (TwWidget* w = m_widgetUnderMouse; w && w != this; w = w->parentWidget())
        {
            //TODO: check if r is able to handle events now
            if (w->isEnabled())
            {
                ev->m_localPos = w->mapFromRoot(ev->clientPos());
                w->processEvent(ev);
                if (ev->accepted())
                {
                    return;
                }
            }
        }
    }
}

void TwRootWidget::disPatchDragEvent(TwDragEvent* ev)
{
    TW_UNUSED(ev);
}

void TwRootWidget::dispatchKeyEvent(TwKeyEvent* ev)
{
    TwWidget* keyEventReciever = pickKeyEventReciever();

    if (keyEventReciever == this)
    {
    }
    else
    {
        for (TwWidget* r = keyEventReciever; r && r != this; r = keyEventReciever->parentWidget())
        {
            r->processEvent(ev);
            if (ev->accepted())
            {
                break;
            }
        }
    }
}

void TwRootWidget::dispatchCharEvent(TwCharEvent* ev)
{
    TwWidget* keyEventReciever = pickKeyEventReciever();

    if (keyEventReciever == this)
    {
    }
    else
    {
        for (TwWidget* r = keyEventReciever; r && r != this; r = keyEventReciever->parentWidget())
        {
            r->processEvent(ev);
            if (ev->accepted())
            {
                break;
            }
        }
    }
}

void TwRootWidget::checkWidgets(std::list<TwWeakPtr<TwWidget> >& saves, const std::list<TwWidget*>& checks, std::list<TwWidget*>& news, std::list<TwWidget*>& erases)
{
    for (auto i = saves.begin(); i != saves.end();)
    {
        if (i->isNull())
        {
            i = saves.erase(i);
        }
        else
        {
            auto it = std::find_if(checks.begin(), checks.end(), [=](TwWidget* w)
            {
                return w == (*i).data();
            });

            if (it == checks.end())
            {
                erases.push_front((*i).data());
                i = saves.erase(i);
            }
            else
            {
                ++i;
            }
        }
    }

    auto itInsert = saves.begin();;
    for (auto i = checks.begin(); i != checks.end(); ++i)
    {
        auto it = std::find_if(itInsert, saves.end(), [=](TwWeakPtr<TwWidget> w)
        {
            return w.data() == *i;
        });

        if (it == saves.end())
        {
            news.push_back(*i);
        }
    }
}


