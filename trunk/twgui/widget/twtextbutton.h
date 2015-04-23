#pragma once

class TW_GUI_API TwTextButton : public TwButton
{
    TW_CLASS_DECLARE(TwTextButton);
public:
    explicit TwTextButton(TwWidget* parent = nullptr);
    ~TwTextButton();

    void setText(const std::wstring& text);
    const std::wstring& text() const;

    void setFont(const TwFont& font);
    const TwFont& font() const;

    virtual TwSize<int> hintSize() const;
    virtual void layoutInternalWidgets();

protected:
    virtual void onEnableChanged();

private:
    TwSimpleTextWidget* m_textWidget;

    TwColor m_textColor;
    TwColor m_disableTextColor;
};

TW_CLASS_REGISTER(TwTextButton);
