#pragma once

struct TW_GUI_API TwFont
{
    std::wstring m_fontFamily;
    float m_size;
    bool m_bold;
    bool m_italic;
    bool m_underline;
    bool m_strikeline;

    TwFont(const std::wstring& fontFamily = L"", float size = 0.0f, bool bold = false, bool italic = false, bool underline = false, bool strikeline = false);

    int compare(const TwFont& other) const;

    int measureWidth(wchar_t c);

    int measureWidth(const std::wstring& text);

    bool operator == (const TwFont& other) const { return compare(other) == 0; }
    bool operator != (const TwFont& other) const { return compare(other) != 0; }
    bool operator <  (const TwFont& other) const { return compare(other) < 0; }
    bool operator <= (const TwFont& other) const { return compare(other) <= 0; }
    bool operator >  (const TwFont& other) const { return compare(other) > 0; }
    bool operator >= (const TwFont& other) const { return compare(other) >= 0; }

};
