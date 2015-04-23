#include "stable.h"
#include "twtextbox.h"
#include "twtextedit.h"

TW_CLASS_IMPLEMENT(TwTextEdit, TwWidget);

TwTextEdit::TwTextEdit(TwWidget*parent /*= nullptr*/)
    : TwScrollView(parent)
    , m_textBox(nullptr)
{
    m_textBox = new TwTextBox(this);
    setContentWidget(m_textBox);
}

TwTextEdit::~TwTextEdit()
{

}

void TwTextEdit::setText(const std::wstring& text)
{
    m_textBox->setText(text);
}

const std::wstring& TwTextEdit::text() const
{
    return m_textBox->text();
}

void TwTextEdit::setFont(const TwFont& font)
{
    m_textBox->setFont(font);
}

void TwTextEdit::setSingleLine(bool )
{

}

void TwTextEdit::setReadOnly(bool readOnly)
{
    m_textBox->setReadOnly(readOnly);
}
