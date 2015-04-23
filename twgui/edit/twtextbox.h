#pragma once

class TW_GUI_API TwTextBox : public TwWidget
{
public:
    explicit TwTextBox(TwWidget*parent = nullptr);
    ~TwTextBox();

    //text
    void setText(const std::wstring& text);
    const std::wstring& text() const;
    void appendText(const std::wstring& text);
    void insertText(int postion, const std::wstring& text);
    void modifyText(int beginPos, int endPos, const std::wstring& text);

    //caret
    void selectText(int beginPos, int endPos);
    void setCaretBegin();
    void setCaretEnd();
    int caretBegin() const;
    int caretEnd() const;

    int rowWidth(int row);
    int rowHeight();
    int textPosFromLocalPoint(TwPoint<int> pt);

    //property
    void setFont(const TwFont& font);

    void setReadOnly(bool readOnly);
    bool isReadOnly() const { return m_readOnly; }

    virtual void paint(TwPainter*);

protected:
    virtual void onMousePresseEvent(TwMouseEvent* ev);
    virtual void onMouseReleaseEvent(TwMouseEvent* ev);
    virtual void onMouseMoveEvent(TwMouseEvent* ev);

    virtual void onKeyPresseEvent(TwKeyEvent* ev);

    virtual void onCharEvent(TwCharEvent* ev);

    virtual void onFocusInEvent(TwFocusEvent* ev);
    virtual void onFocusOutEvent(TwFocusEvent* ev);

private:
    std::wstring m_text;
    bool m_readOnly;
    bool m_pressed;
};