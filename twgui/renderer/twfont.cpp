#include "stable.h"
#include "twfont.h"

TwFont::TwFont(const std::wstring& fontFamily /*= L""*/, float size /*= 0.0f*/, bool bold /*= false*/, bool italic /*= false*/, bool underline /*= false*/, bool strikeline /*= false*/)
    : m_fontFamily(fontFamily)
    , m_size(size)
    , m_bold(bold)
    , m_italic(italic)
    , m_underline(underline)
    , m_strikeline(strikeline)
{

}

int TwFont::compare(const TwFont& other) const
{
    int result = 0;
    do
    {
        result = m_fontFamily.compare(other.m_fontFamily);
        if (result != 0)
            break;

        result = (int)(m_size - other.m_size);
        if (result != 0)
            break;

        result = (int)m_bold - (int)other.m_bold;
        if (result != 0)
            break;

        result = (int)m_italic - (int)other.m_italic;
        if (result != 0)
            break;

        result = (int)m_underline - (int)other.m_underline;
        if (result != 0)
            break;

        result = (int)m_strikeline - (int)other.m_strikeline;
        if (result != 0)
            break;

    } while (false);

    return result;
}

int TwFont::measureWidth(wchar_t c)
{
    return twApp()->rendererResourceProvider()->getTextMeasure(*this)->measureText(c).width();
}

int TwFont::measureWidth(const std::wstring& text)
{
    return twApp()->rendererResourceProvider()->getTextMeasure(*this)->measureText(text).width();
}


