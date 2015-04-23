#pragma once
#include "twtype.h"

class TW_BASE_API TwColor
{

public:
    TwColor(uint8 r, uint8 g, uint8 b, uint8 a = 255);
    TwColor();
    explicit TwColor(uint rgba);

    enum ColorName
    {
        White,
        Snow,
        LightBlue,
        LightSkyBlue,
        LightGrey,
        SilverChalice,
        Grey,
        Grey61,
        Grey51,
        Dark,
        Black
    };
    static TwColor twColor(ColorName);

    void setColor(uint8 r, uint8 g, uint8 b, uint8 a = 255);
    void setColor(uint rgba);

    void setRed(uint8 r) { m_red = r;}
    uint8 red() const { return m_red; }

    void setGreen(uint8 g) { m_green = g; }
    uint8 green() const { return m_green; }

    void setBlue(uint8 b) { m_blue = b; }
    uint8 blue() const { return m_blue;}

    void setAlpha(uint8 a) { m_alpha = a; }
    uint8 alpha() const { return m_alpha; }

    bool operator == (const TwColor& color) const
    {
        return m_red   == color.m_red
            && m_green == color.m_green
            && m_blue  == color.m_blue
            && m_alpha == color.m_alpha;
    }

    bool operator != (const TwColor& color) const
    {
        return m_red   != color.m_red
            || m_green != color.m_green
            || m_blue  != color.m_blue
            || m_alpha != color.m_alpha;
    }

private:
    uint8 m_red;
    uint8 m_green;
    uint8 m_blue;
    uint8 m_alpha;
};
