#pragma once

class TwTextBox;

class TW_GUI_API TwTextEdit : public TwScrollView
{
    TW_CLASS_DECLARE(TwTextEdit);
public:
    explicit TwTextEdit(TwWidget*parent = nullptr);
    ~TwTextEdit();

    void setText(const std::wstring& text);
    const std::wstring& text() const;

    void setFont(const TwFont& font);

    void setSingleLine(bool singleLine);
    void setReadOnly(bool readOnly);

protected:

private:
    TwTextBox* m_textBox;
};

TW_CLASS_REGISTER(TwTextEdit);
