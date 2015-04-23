#pragma once

class TW_GUI_API TwScrollBar : public TwWidget
{
    friend class TwScrollView;

    TW_CLASS_DECLARE(TwScrollBar);
public:
    explicit TwScrollBar(TwWidget* parent = nullptr);
    ~TwScrollBar();

    void setRange(float min, float max);
    float range() const { return m_maxPostion - m_minPostion; }

    void setPageStep(float step);
    float pageStep() const { return m_pageStep; }

    void setSingleStep(float step);
    float singleStep() const { return m_singleStep; }

    void scrollToPostion(float postion);
    float currentPostion() const { return m_currentPostion; }

    void scroll(float step); //
    void scrollUp(float step);
    void scrollDown(float step);

    void scrollToMin();
    void scrollToMax();

    void setOrientation(Tw::Orientation o);
    void setAutoHide(bool autoHide);

    virtual void layoutInternalWidgets();

    virtual TwSize<int> hintSize() const;

    virtual void paintBackground(TwPainter*);
    virtual void paint(TwPainter*);

public:
    TwSignal<void(float)> sigScrollPostionChanged;
    TwSignal<void()> sigPressed;

protected:
    void onMinButtonClicked(TwObject* sender);
    void onMaxButtonClicked(TwObject* sender);

    void onMinButtonPressed(TwObject* sender);
    void onMaxButtonPressed(TwObject* sender);

    void onMinButtonReleased(TwObject* sender);
    void onMaxButtonReleased(TwObject* sender);

    void startAutoScrollTimer(Tw::Direction direction, bool autoModeSingleStep);
    void stopAutoScrollTimer();
    void onAutoScrollTimerUpdate(TwObject*);

    void updateSlider();
    virtual void updateSliderRect();

    virtual void onPostionChanged(float oldPostion);

    virtual void onResizeEvent(TwResizeEvent* ev);

    virtual void onMousePresseEvent(TwMouseEvent* ev);
    virtual void onMouseReleaseEvent(TwMouseEvent* ev);
    virtual void onMouseMoveEvent(TwMouseEvent* ev);

    virtual void onHoverEnterEvent(TwHoverEvent* ev);
    virtual void onHoverLeaveEvent(TwHoverEvent* ev);

    virtual void onWheelEvent(TwWheelEvent* ev);

protected:
    TwButton* m_minButton;
    TwButton* m_maxButton;
    Tw::Orientation m_orientation;

    float m_minPostion;
    float m_maxPostion;
    float m_currentPostion;
    float m_pageStep;
    float m_singleStep;

    TwRect<int> m_sliderRect;

    TwPoint<int> m_dragStartPoint;
    float m_dragStartPostion;
    bool m_isDragSlider;
    bool m_hovered;

    TwTimer m_autoScrollTimer;
    Tw::Direction m_autoDirection;
    bool m_autoModeSingleStep;
    bool m_autoHide;

};

TW_CLASS_REGISTER(TwScrollBar);
