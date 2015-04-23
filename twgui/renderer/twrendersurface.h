#pragma once

class TwWindow;
struct  ITwRenderSurface
{
    virtual ~ITwRenderSurface() {;}

    virtual TwSize<int> size() const = 0;
    virtual void resize(const TwSize<int>& sz) = 0;

    virtual void translate(const TwPoint<int>& offset) = 0;
    virtual void rotate(float angle) = 0;
    virtual void scale(float factorX, float factorY) = 0;
    virtual void shear(float sx, float sy) = 0;

    virtual void pushClipRect(const TwRect<int>& rc) = 0;
    virtual void popClipRect() = 0;
    virtual void setBrushColor(const TwColor& color) = 0;
    virtual TwColor brushColor() const = 0;

    virtual void saveState() = 0;
    virtual void restoreState() = 0;

    virtual void beginRender() = 0;
    virtual void endRender() = 0;

    virtual void clear(const TwColor& color) = 0;

    virtual void drawLine(const TwPoint<int>& pt1, const TwPoint<int>& pt2, const TwColor& color, bool adjustBorderLine = false) = 0;

    virtual void drawRect(const TwRect<int>& rc, const TwColor& color, float width = 1.0, bool adjustBorderLine = false) = 0;
    virtual void fillRect(const TwRect<int>& rc, const TwColor& color) = 0;

    virtual void drawRoundedRect(const TwRect<int>& rc, int xradius, int yradius, const TwColor& color, float width = 1.0, bool adjustBorderLine = false) = 0;
    virtual void fillRoundedRect(const TwRect<int>& rc, int xradius, int yradius, const TwColor& color) = 0;

    virtual void drawText(const std::wstring& text, const TwRect<int>& rc ) = 0;
    virtual void drawText(const std::wstring& text, const TwRect<int>& rc, const TwColor& color ) = 0;
    virtual void drawText(const std::wstring& text, const TwRect<int>& rc, const TwFont& font) = 0;

    virtual void drawImage(const TwImage& image, const TwPoint<int>& pt) = 0;
    virtual void drawImage(const TwImage& image, const TwRect<int>& destRc) = 0;
    virtual void drawImage(const TwImage& image, const TwRect<int>& destRc, const TwRect<int>& srcRc) = 0;
};
