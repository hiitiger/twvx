#include "stable.h"

#include "twlabel.h"

TW_CLASS_IMPLEMENT(TwLabel,TwWidget);

TwLabel::TwLabel(TwWidget* parent /*= nullptr*/)
    : TwWidget(parent)
    , sigClicked(this)
    , m_textWidget(nullptr)
    , m_acceptClick(false)
    , m_pressed(false)
    , m_hovered(false)
{
    m_textWidget = new TwSimpleTextWidget(this);
    m_textWidget->setFont(TwFont(L"Microsoft YaHei UI", 16));
    m_textWidget->setTextAlignment(Tw::Align_Center, Tw::Align_Center);
    m_textWidget->setCursor(TwCursor(TwCursor::HandCursor));
    m_textWidget->installEventHook(this);

    setTextColor(TwColor(0, 0, 0));
    setHilightTextColor(TwColor(46, 175, 245));
}

void TwLabel::setText(const std::wstring& text)
{
    m_textWidget->setText(text);
    updateBound();
}

const std::wstring& TwLabel::text() const
{
    return m_textWidget->text();
}

void TwLabel::setFont(const TwFont& font)
{
    m_textWidget->setFont(font);
    updateBound();
}

const TwFont& TwLabel::font() const
{
    return m_textWidget->font();
}

void TwLabel::setTextColor(const TwColor& color)
{
    m_normalColor = color;
    if (!m_pressed)
    {
        m_textWidget->setTextColor(color);
    }
}

void TwLabel::setHilightTextColor(const TwColor& color)
{
    m_hilightTextColor = color;
    if (m_pressed)
    {
        m_textWidget->setTextColor(color);
    }
}

void TwLabel::setAcceptClick(bool acceptClick)
{
    if (m_acceptClick != acceptClick)
    {
        m_acceptClick = acceptClick;

        if (m_acceptClick)
        {
            m_textWidget->setAcceptHoverEvent(true);
        }
        else
        {
            m_hovered = false;
            m_pressed = false;
            m_textWidget->setUnderline(false);
            m_textWidget->setAcceptHoverEvent(false);
        }
    }
}

bool TwLabel::acceptClick() const
{
    return m_acceptClick;
}

TwSize<int> TwLabel::hintSize() const
{
    return m_textWidget->hintSize();
}

void TwLabel::layoutInternalWidgets()
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

void TwLabel::onEnableChanged()
{
    m_textWidget->setEnabled(isEnabled());
    if (isEnabled())
    {
        m_textWidget->setUnderline(m_pressed);
        m_textWidget->setTextColor(m_hovered ? m_hilightTextColor : m_normalColor);
    }
    else
    {
        m_textWidget->setUnderline(false);
        m_textWidget->setTextColor(TwColor::twColor(TwColor::SilverChalice));
    }
    TwWidget::onEnableChanged();
}

bool TwLabel::hookHandleEvent(TwEventPort* w, TwEvent* ev)
{
    if (w != m_textWidget)
    {
        return false;
    }

    if (ev->id() == Tw::Ev_HoverEnter)
    {
        m_hovered = true;
        m_textWidget->setUnderline(true);
        m_textWidget->setTextColor(m_hilightTextColor);
    }
    else if (ev->id() == Tw::Ev_HoverLeave)
    {
        m_hovered = false;
        m_textWidget->setUnderline(false);
        m_textWidget->setTextColor(m_normalColor);
    }
    else if (ev->id() == Tw::Ev_MousePress)
    {
        TwMouseEvent* mev = static_cast<TwMouseEvent*>(ev);

        if (m_textWidget->localRect().contains(mev->localPos()))
        {
            ev->accept();
            m_pressed = true;
        }
        return true;
    }
    else if (ev->id() == Tw::Ev_MouseRelease)
    {
        TwMouseEvent* mev = static_cast<TwMouseEvent*>(ev);
        m_pressed = false;

        ev->accept();

        if (m_textWidget->localRect().contains(mev->localPos()))
        {
            sigClicked.emit();
        }
        return true;
    }
    else if (ev->id() == Tw::Ev_MouseMove)
    {
        TwMouseEvent* mev = static_cast<TwMouseEvent*>(ev);
        m_pressed = false;

        if (m_textWidget->localRect().contains(mev->localPos()))
        {
            m_hovered = true;
            m_textWidget->setUnderline(true);
            m_textWidget->setTextColor(m_hilightTextColor);
        }
        else
        {
            m_hovered = false;
            m_textWidget->setUnderline(false);
            m_textWidget->setTextColor(m_normalColor);
        }
        return true;
    }
    return false;
}

