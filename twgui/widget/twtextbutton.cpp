#include "stable.h"
#include "twtextbutton.h"


TW_CLASS_IMPLEMENT(TwTextButton, TwButton);

TwTextButton::TwTextButton(TwWidget* parent /*= nullptr*/)
    : TwButton(parent)
    , m_textWidget(nullptr)
    , m_textColor(0,0,0)
    , m_disableTextColor(TwColor::twColor(TwColor::SilverChalice))
{
    m_textWidget = new TwSimpleTextWidget(this);
    m_textWidget->setFont(TwFont(L"Microsoft YaHei UI", 16));
    m_textWidget->setTextAlignment(Tw::Align_Center, Tw::Align_Center);
}

TwTextButton::~TwTextButton()
{

}

void TwTextButton::setText(const std::wstring& text)
{
    m_textWidget->setText(text);
    updateBound();
}

const std::wstring& TwTextButton::text() const
{
    return m_textWidget->text();
}

void TwTextButton::setFont(const TwFont& font)
{
    m_textWidget->setFont(font);
    updateBound();
}

const TwFont& TwTextButton::font() const
{
    return m_textWidget->font();
}

TwSize<int> TwTextButton::hintSize() const
{
    return m_textWidget->hintSize();
}

void TwTextButton::layoutInternalWidgets()
{
    TwSize<int> textSize = m_textWidget->hintSize();
    if (textSize.width() < width())
    {
        int x = (width() - textSize.width()) / 2;
        int y = (height() - textSize.height()) / 2;
        TwRect<int> textRect(TwPoint<int>(x, y), textSize);
        m_textWidget->setBoundRect(textRect);
    }
    else
    {
        textSize.setWidth(width());
        TwRect<int> textRect(TwPoint<int>(0, 0), textSize);
        m_textWidget->setBoundRect(textRect);
    }
}

void TwTextButton::onEnableChanged()
{
    if (isEnabled())
    {
        m_textWidget->setTextColor(m_textColor);
    }
    else
    {
        m_textWidget->setTextColor(m_disableTextColor);
    }
    TwButton::onEnableChanged();
}
