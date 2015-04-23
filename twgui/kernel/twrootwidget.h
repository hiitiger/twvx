#pragma once

/*
    RootWidget will dispatch widget events
*/


class TW_GUI_API TwRootWidget : public TwWidget
{
public:
    explicit TwRootWidget(TwWindow* window = nullptr);
    virtual ~TwRootWidget();

    TwWindow* window() const;
    void setContentWidget(TwWidget* contentWidget,bool deleteOld = true) ;
    TwWidget* contentWidget() const;

    virtual void updateBound();

    void schedulePaintWidget(TwWidget* w);

    TwWidget* resetWidgetUnderMouse(const TwPoint<int>& pt);
    TwWidget* pickKeyEventReciever();
    TwWidget* pickFocusWidget();

    void setFocosWidget(TwWidget* w);
    TwWidget* focusWidget() const;

    void dispatchHitTestEvent(TwHitTestEvent* ev);
    void dispatchMouseEvent(TwMouseEvent* ev);

    void disPatchHoverEvent(TwHoverEvent* ev);
    void disPatachWheelEvent(TwWheelEvent* ev);
    void disPatchDragEvent(TwDragEvent* ev);

    void dispatchKeyEvent(TwKeyEvent* ev);
    void dispatchCharEvent(TwCharEvent* ev);

protected:
    void checkWidgets(std::list<TwWeakPtr<TwWidget> >& saves,const std::list<TwWidget*>& checks, std::list<TwWidget*>& news, std::list<TwWidget*>& erases);

private:
    TwWindow* m_twWindow;
    TwWidget* m_contentWidget;


    //events handler
    TwWeakPtr<TwWidget> m_widgetUnderMouse;
    TwWeakPtr<TwWidget> m_mousePressHandler;
    std::list<TwWeakPtr<TwWidget> > m_hoverHandlers;

    TwWeakPtr<TwWidget> m_focusWidget;
};