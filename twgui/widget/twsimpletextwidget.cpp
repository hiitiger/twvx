#include "stable.h"
#include "twsimpletextwidget.h"

TW_CLASS_IMPLEMENT(TwSimpleTextWidget, TwWidget);

TwSimpleTextWidget::TwSimpleTextWidget(TwWidget* parent /*= nullptr*/)
    : TwWidget(parent)
{
    m_textRender.setFont(TwFont(L"Microsoft YaHei UI", 16));
    m_textRender.setTextAlignment(Tw::Align_Left, Tw::Align_Top);
}

void TwSimpleTextWidget::setText(const std::wstring& text)
{
    m_textRender.setText(text);
    updateBound();
}

const std::wstring& TwSimpleTextWidget::text() const
{
    return m_textRender.text();
}

void TwSimpleTextWidget::setFont(const TwFont& font)
{
    m_textRender.setFont(font);
    updateBound();
}

const TwFont& TwSimpleTextWidget::font() const
{
    return m_textRender.font();
}

void TwSimpleTextWidget::setFontSize(float size)
{
    m_textRender.setFontSize(size);
    updateBound();
}

void TwSimpleTextWidget::setBold(bool bold)
{
    m_textRender.setBold(bold);
    updateBound();
}

void TwSimpleTextWidget::setItalic(bool italic)
{
    m_textRender.setItalic(italic);
    updateBound();
}

void TwSimpleTextWidget::setUnderline(bool underline)
{
    m_textRender.setUnderline(underline);
    update();
}

void TwSimpleTextWidget::setStrikeline(bool strikeline)
{
    m_textRender.setStrikeline(strikeline);
    update();
}

void TwSimpleTextWidget::setAutoMultiLine(bool multiline)
{
    m_textRender.setAutoMultiLine(multiline);
    updateBound();
}

void TwSimpleTextWidget::paint(TwPainter* painter)
{
    m_textRender.setRenderSurface(painter->renderSurface());
    m_textRender.render(localRect());
}

void TwSimpleTextWidget::setTextAlignment(Tw::Alignment horizontalAlign, Tw::Alignment verticalAlign)
{
    m_textRender.setTextAlignment(horizontalAlign, verticalAlign);
    updateBound();
}

void TwSimpleTextWidget::setTextColor(const TwColor& color)
{
    m_textRender.setTextColor(color);
    update();
}

TwSize<int> TwSimpleTextWidget::hintSize() const
{
    if (m_textRender.autoMultiLine())
    {
        return TwSize<int>();
    }
    else
    {
        return m_textRender.minSize();
    }
}

