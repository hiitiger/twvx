#pragma once
class ITwRenderer
{
public:
    virtual ~ITwRenderer(){;}
    virtual void setRenderSurface(ITwRenderSurface* renderSurface) = 0;
    virtual void render(TwRect<int> rect) = 0;
};

//TODO:
class TW_GUI_API TwSimpleTextRenderer : public ITwRenderer
{
public:
    TwSimpleTextRenderer();

    ~TwSimpleTextRenderer();

    void setText(const std::wstring& text)
    {
        if (m_text != text)
        {
            m_text = text;
            updateRenderer();
        }
    }

    const std::wstring& text() const
    {
        return m_text;
    }
 
    void setFont(const TwFont& font)
    {
        if (m_font != font)
        {
            m_font = font;
            updateRenderer();
        }
    }

    const TwFont& font() const
    {
        return m_font;
    }

    void setFontSize(float size)
    {
        if (m_font.m_size != size)
        {
            m_font.m_size = size;
            updateRenderer();
        }
    }

    void setBold(bool bold)
    {
        if (m_font.m_bold != bold)
        {
            m_font.m_bold = bold;
            updateRenderer();
        }
    }

    void setItalic(bool italic)
    {
        if (m_font.m_italic != italic)
        {
            m_font.m_italic = italic;
            updateRenderer();
        }
    }

    void setUnderline(bool underline)
    {
        if (m_font.m_underline != underline)
        {
            m_font.m_underline = underline;
            updateRenderer();
        }
    }

    void setStrikeline(bool strikeline)
    {
        if (m_font.m_strikeline != strikeline)
        {
            m_font.m_strikeline = strikeline;
            updateRenderer();
        }
    }

    void setTextColor(TwColor color)
    {
        m_color = color;
    }

    TwColor textColor() const
    {
        return m_color;
    }

    void setTextAlignment(Tw::Alignment horizontalAlign, Tw::Alignment verticalAlign)
    {
        if (m_halign != horizontalAlign || m_valign != verticalAlign)
        {
            m_halign = horizontalAlign;
            m_valign = verticalAlign;
            updateRenderer();
        }
    }

    Tw::Alignment hAlign() const
    {
        return m_halign;
    }
 
    Tw::Alignment vAlign() const
    {
        return m_valign;
    }

    void setAutoMultiLine(bool m)
    {
        if (m_autoMultiLine != m)
        {
            m_autoMultiLine = m;
            updateRenderer();
        }
    }

    bool autoMultiLine() const
    {
        return m_autoMultiLine;
    }

    virtual void setRenderSurface(ITwRenderSurface* renderSurface);
    virtual void render(TwRect<int> rc);

    void updateRenderer();
    void createTextFormat();
    void createTextLayout();
    TwSize<int> minSize() const;

private:
    TwD2DRenderSurface* m_D2DSurface;
    WinComPtr<IDWriteTextFormat> m_textFormat;
    WinComPtr<IDWriteTextLayout> m_textLayout;

    std::wstring m_text;
    TwFont m_font;
    TwColor m_color;
    Tw::Alignment m_halign;
    Tw::Alignment m_valign;
    bool m_autoMultiLine;
    TwSize<int> m_minSize;

};