#pragma once

class TW_GUI_API TwLabel : public TwWidget
{
    TW_CLASS_DECLARE(TwLabel);

public:
    explicit TwLabel(TwWidget* parent = nullptr);

    void setText(const std::wstring& text);
    const std::wstring& text() const;

    void setFont(const TwFont& font);
    const TwFont& font() const;

    void setTextColor(const TwColor& color);
    void setHilightTextColor(const TwColor& color);

    void setAcceptClick(bool acceptClick);
    bool acceptClick() const;

    virtual TwSize<int> hintSize() const;
    virtual void layoutInternalWidgets();

public: //signals:
    TwSignal<void()> sigClicked;

protected:
    virtual void onEnableChanged();

    virtual bool hookHandleEvent(TwEventPort*, TwEvent*);

private:
    TwSimpleTextWidget* m_textWidget;
    TwColor m_normalColor;
    TwColor m_hilightTextColor;
    bool m_acceptClick;
    bool m_pressed;
    bool m_hovered;
};

TW_CLASS_REGISTER(TwLabel);
