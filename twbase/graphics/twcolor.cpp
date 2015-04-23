#include "stable.h"
#include "twcolor.h"

namespace
{
    static const uint sc_redShift   = 24;
    static const uint sc_greenShift = 16;
    static const uint sc_blueShift  = 8;
    static const uint sc_alphaShitf = 0;

    static const uint sc_redMask    = 0xff << sc_redShift;
    static const uint sc_greenMask  = 0xff << sc_greenShift;
    static const uint sc_blueMask   = 0xff << sc_blueShift;
    static const uint sc_alphaMask  = 0xff << sc_alphaShitf;
};


#define TWCOLOR_TABLE_COLOR(name, rgb) \
    TwColor::##name, rgb##FF

const struct ColorTable{
    TwColor::ColorName name;
    uint rgba;
} colorTable[] = 
{
    TWCOLOR_TABLE_COLOR(White, 0xFFFFFF),
    TWCOLOR_TABLE_COLOR(Snow, 0xFFFAFA),
    TWCOLOR_TABLE_COLOR(LightBlue, 0xBFEFFF),
    TWCOLOR_TABLE_COLOR(LightSkyBlue, 0x87CEFA),
    TWCOLOR_TABLE_COLOR(LightGrey, 0xD3D3D3),
    TWCOLOR_TABLE_COLOR(SilverChalice, 0xACACAC),
    TWCOLOR_TABLE_COLOR(Grey, 0x808080),
    TWCOLOR_TABLE_COLOR(Grey61, 0x9C9C9C),
    TWCOLOR_TABLE_COLOR(Grey51, 0x828282),
    TWCOLOR_TABLE_COLOR(Dark, 0x303030),
    TWCOLOR_TABLE_COLOR(Black, 0x000000),

};


TwColor TwColor::twColor(ColorName name)
{
    return TwColor(colorTable[name].rgba);
}

TwColor::TwColor( uint8 r, uint8 g, uint8 b, uint8 a /*= 255*/ )
    : m_red(r), m_green(g), m_blue(b), m_alpha(a)
{

}

TwColor::TwColor()
    : m_red(0), m_green(0), m_blue(0), m_alpha(255)
{

}

TwColor::TwColor(uint rgba)
{
    setColor(rgba);
}

void TwColor::setColor( uint8 r, uint8 g, uint8 b, uint8 a /*= 255*/ )
{
    m_red   = r;
    m_green = g;
    m_blue  = b;
    m_alpha = a;
}

void TwColor::setColor(uint rgba)
{
    m_red   = static_cast<uint8>((rgba & sc_redMask) >> sc_redShift);
    m_green = static_cast<uint8>((rgba & sc_greenMask) >> sc_greenShift);
    m_blue  = static_cast<uint8>((rgba & sc_blueMask) >> sc_blueShift);
    m_alpha = static_cast<uint8>((rgba & sc_alphaMask) >> sc_alphaShitf);
}
