#pragma once

class TW_GUI_API TwSlider : public TwWidget
{
    TW_CLASS_DECLARE(TwSlider);
public:
    explicit TwSlider(TwWidget* parent = nullptr);
    ~TwSlider();

    void setCurrentValue(float value);
    float currentValue() const { return m_currentValue; }

    void setRange(float min, float max);
    float range() const { return m_maxValue - m_minValue; }

    virtual TwSize<int> hintSize() const;

    virtual void paintBackground(TwPainter*);
    virtual void paint(TwPainter*);

public:
    TwSignal<void(float)> sigValueChanged;

protected:
    virtual void onCurrentValueChanged(float oldValue);

    virtual void onResizeEvent(TwResizeEvent* ev);

    virtual void onMousePresseEvent(TwMouseEvent* ev);
    virtual void onMouseReleaseEvent(TwMouseEvent* ev);
    virtual void onMouseMoveEvent(TwMouseEvent* ev);

    virtual void onHoverEnterEvent(TwHoverEvent* ev);
    virtual void onHoverLeaveEvent(TwHoverEvent* ev);

private:
    float m_minValue;
    float m_maxValue;
    float m_currentValue;

    TwRect<int> m_sliderRect;
    TwPoint<int> m_dragStartPoint;
    float m_dragStartValue;
    bool m_isDragSlider;

    bool m_hovered;
};

TW_CLASS_REGISTER(TwSlider);
