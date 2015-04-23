#pragma once

class TW_GUI_API TwScrollView : public TwWidget
{
    friend class TwScrollBar;

    TW_CLASS_DECLARE(TwScrollView);
public:
    explicit TwScrollView(TwWidget* parent = nullptr);
    ~TwScrollView();

    void setContentWidget(TwWidget* w);

    void setSingleStep(float singleStep);

    void scrollToPostion(float postion);

    void scrollToMin();

    void scrollToMax();

    virtual TwSize<int> hintSize() const;

protected:
    virtual void onScrollVisbleChanged(TwObject*);
    virtual void onScrollBarPostionChanged(TwObject*, float);
    virtual void onScrollBarPressed(TwObject*);

    virtual void onContentWidgetChanged();
    virtual void onContentWidgetBoundChanged(TwObject*);

    virtual void updateScrollView();
    virtual void updateContentVisibleBound(const TwRect<int>& rc);

    virtual bool hookHandleEvent(TwEventPort* p, TwEvent* ev);

    virtual void onResizeEvent(TwResizeEvent* ev);
    virtual void onWheelEvent(TwWheelEvent* ev);

private:
    TwScrollBar* m_scrollBar;

    TwWidget* m_contentWidget;
    std::shared_ptr<TwSignalConnectionBase> m_contentBoundChangedSigConnection;
};

TW_CLASS_REGISTER(TwScrollView);
