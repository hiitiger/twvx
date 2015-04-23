#pragma once
#include "twtextmeasure.h"
#include "twfont.h"
#include "twrenderresourceprovider.h"
#include "twrendersurface.h"
#include "twrendersufaced2d.h"
#include "twrender.h"

class TW_GUI_API TwPainter
{
public:
    explicit TwPainter(ITwRenderSurface* surface);
    virtual ~TwPainter();
    ITwRenderSurface* renderSurface() const { return m_surface; }

    static bool initRenderSys();
    static void unitRenderSys();

    void translate(const TwPoint<int>& offset);
    void translate(int x, int y);
    void rotate(float angle);
    void scale(float factorX, float factorY);
    void shear(float sx, float sy);

    void pushClipRect(const TwRect<int>& rc);
    void popClipRect();
    TwRect<int> lastPushedClipRect();

    void setBrushColor(const TwColor& color);
    TwColor brushColor() const;

    void saveState();
    void restoreState();

    void beginRender();
    void endRender();

    //TODO
    void beginLayer(float transparency, const TwRect<int>& bounds);
    void endLayer();

    void paint(ITwRenderer*, const TwRect<int>& rc );

    void drawLine(const TwPoint<int>& pt1, const TwPoint<int>& pt2, const TwColor& color, bool adjustBorderLine = false);

    void drawRect(const TwRect<int>& rc, const TwColor& color, float width = 1.0, bool adjustBorderLine = false);
    void fillRect(const TwRect<int>& rc, const TwColor& color);
    
    void drawRoundedRect(const TwRect<int>& rc, int xradius, int yradius, const TwColor& color, float width = 1.0, bool adjustBorderLine = false);
    void fillRoundedRect(const TwRect<int>& rc, int xradius, int yradius, const TwColor& color);

    void drawText(const std::wstring& text, const TwRect<int>& rc );
    void drawText(const std::wstring& text, const TwRect<int>& rc, const TwColor& color );
    void drawText(const std::wstring& text, const TwRect<int>& rc, const TwFont& font);

    void drawImage(const TwImage& image, const TwPoint<int>& pt);
    void drawImage(const TwImage& image, const TwRect<int>& destRc);
    void drawImage(const TwImage& image, const TwRect<int>& destRc, const TwRect<int>& srcRc);

private:
    ITwRenderSurface* m_surface;

    std::deque<TwRect<int>> m_clips;
    std::deque<int> m_clipCounter;
};