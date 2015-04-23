#include "stable.h"

#include "twtextmeasure.h"

TwTextMeasure::TwTextMeasure()
{
    for (auto i : m_cacheCharSize)
    {
        i = TwSize<int>(-1, -1);
    }
}

TwSize<int> TwTextMeasure::measureText(wchar_t c)
{
    if (m_cacheCharSize[c].isValid())
    {
        return m_cacheCharSize[c];
    }
    else
    {
        m_cacheCharSize[c] = measureTextInternal(c);
        return m_cacheCharSize[c];
    }
}

TwSize<int> TwTextMeasure::measureText(const std::wstring& text)
{
    return measureTextInternal(text);
}
