#pragma once

class TW_GUI_API TwTextMeasure : public TwRefObject<TwRefBase>
{
public:
    TwTextMeasure();

    TwSize<int> measureText(wchar_t c);
    TwSize<int> measureText(const std::wstring& text);

protected:
    virtual TwSize<int> measureTextInternal(wchar_t c) = 0;
    virtual TwSize<int> measureTextInternal(const std::wstring& text) = 0;

private:
    TwSize<int> m_cacheCharSize[65536];
};
