#pragma once

class TW_GUI_API TwProgressBar : public TwWidget
{
    TW_CLASS_DECLARE(TwProgressBar);
public:
    explicit TwProgressBar(TwWidget* parent = nullptr);
    ~TwProgressBar();

    void setProgressValue(float progress);
    float progressValue() const { return m_progressValue; }

    void setProgressColor(TwColor color);

    virtual TwSize<int> hintSize() const;

    virtual void paintBackground(TwPainter*);
    virtual void paint(TwPainter*);

public:
    TwSignal<void(float)> sigProgressChanged;
    TwSignal<void()> sigProgressCompleted;

protected:
    virtual void onProgressChanged(float oldProgress) { TW_UNUSED(oldProgress); }

private:
    TwColor m_color;

    float m_progressValue;
};

TW_CLASS_REGISTER(TwProgressBar);
