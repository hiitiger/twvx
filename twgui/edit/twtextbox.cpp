#include "stable.h"

#include "twtextbox.h"

TwTextBox::TwTextBox(TwWidget*parent /*= nullptr*/)
    : TwWidget(parent)
{

}

TwTextBox::~TwTextBox()
{

}

void TwTextBox::setText(const std::wstring& text)
{
    if (m_text != text)
    {
        m_text = text;

    }
}

const std::wstring& TwTextBox::text() const
{
    return m_text;
}

void TwTextBox::appendText(const std::wstring& )
{

}

void TwTextBox::insertText(int postion, const std::wstring& text)
{

}

void TwTextBox::modifyText(int beginPos, int endPos, const std::wstring& text)
{

}

void TwTextBox::selectText(int beginPos, int endPos)
{

}

void TwTextBox::setFont(const TwFont& )
{

}

void TwTextBox::setReadOnly(bool)
{

}

void TwTextBox::paint(TwPainter*)
{

}

void TwTextBox::onMousePresseEvent(TwMouseEvent* ev)
{
    if (ev->button() == Tw::MB_LeftButton)
    {
        ev->accept();
        m_pressed = true;
        //
    }
    else if (ev->button() == Tw::MB_RightButton)
    {
        ev->accept();
    }
}

void TwTextBox::onMouseReleaseEvent(TwMouseEvent* ev)
{
    m_pressed = false;

    if (ev->button() == Tw::MB_RightButton)
    {
        //
    }
}

void TwTextBox::onMouseMoveEvent(TwMouseEvent* ev)
{

}

void TwTextBox::onKeyPresseEvent(TwKeyEvent* ev)
{

}

void TwTextBox::onCharEvent(TwCharEvent* ev)
{

}

void TwTextBox::onFocusInEvent(TwFocusEvent* ev)
{

}

void TwTextBox::onFocusOutEvent(TwFocusEvent* ev)
{

}

