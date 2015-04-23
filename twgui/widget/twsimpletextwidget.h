#pragma once

class TW_GUI_API TwSimpleTextWidget : public TwWidget
{
    TW_CLASS_DECLARE(TwSimpleTextWidget);
public:
    explicit TwSimpleTextWidget(TwWidget* parent = nullptr);

    void setText(const std::wstring& text);
    const std::wstring& text() const;

    void setFont(const TwFont& font);
    const TwFont& font() const;

    void setFontSize(float size);
    void setBold(bool);
    void setItalic(bool);
    void setUnderline(bool underline);
    void setStrikeline(bool strikeline);

    void setAutoMultiLine(bool multiline);
    void setTextAlignment(Tw::Alignment horizontalAlign, Tw::Alignment verticalAlign);
    void setTextColor(const TwColor& color);

    virtual TwSize<int> hintSize() const;

protected:
    virtual void paint( TwPainter* painter);

private:

    TwSimpleTextRenderer m_textRender;
};

TW_CLASS_REGISTER(TwSimpleTextWidget);
