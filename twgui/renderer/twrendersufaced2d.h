#pragma once


#include "twrendersurface.h"

inline D2D1::ColorF twColorToD2D1ColorF(TwColor color)
{
    return D2D1::ColorF(static_cast<float>(color.red())/255.f, static_cast<float>(color.green())/255.f, static_cast<float>(color.blue())/255.f, static_cast<float>(color.alpha())/255.f);
}

inline TwColor D2D1ColorFToTwColor(D2D1::ColorF color)
{
    return TwColor( static_cast<int>(color.r * 255.0) , static_cast<int>(color.g * 255.0), static_cast<int>(color.b * 255.0), static_cast<int>(color.a * 255.0));
}

inline TwColor D2D1ColorFToTwColor(D2D1_COLOR_F color)
{
    return TwColor(static_cast<int>(color.r * 255.0), static_cast<int>(color.g * 255.0), static_cast<int>(color.b * 255.0), static_cast<int>(color.a * 255.0));
}


inline D2D1_POINT_2F twPointToD2D1PointF(TwPoint<int> pt)
{
    return D2D1::Point2F(static_cast<float>(pt.x()), static_cast<float>(pt.y()));
}

inline D2D1_POINT_2F twPointToD2D1PointF(TwPoint<float> pt)
{
    return D2D1::Point2F(pt.x(), pt.y());
}

inline D2D1_RECT_F twRectToD2D1RectF(TwRect<int> rc)
{
    return D2D1::RectF(static_cast<float>(rc.left()), static_cast<float>(rc.top()), static_cast<float>(rc.right()), static_cast<float>(rc.bottom()));
}

inline D2D1_RECT_F twRectToD2D1RectF(TwRect<float> rc)
{
    return D2D1::RectF(rc.left(), rc.top(), rc.right(), rc.bottom());
}

inline D2D1_SIZE_U twSizeToD2D1SizeU(TwSize<int> size)
{
    return D2D1::SizeU(size.width(), size.height());
}

inline D2D1::Matrix3x2F twMatrixFormToD2d1Matrix(TwMatrix m)
{
    return D2D1::Matrix3x2F(m._11, m._12, m._21, m._22, m._31, m._32);
}

//TODO: DC Target,  TransparencyLayer...

class TW_GUI_API TwD2DRenderSurface : public ITwRenderSurface
{
public:
    explicit TwD2DRenderSurface(TwWindow* window);
    virtual ~TwD2DRenderSurface();

    ID2D1RenderTarget* getD2DRenderTarget() const { return m_renderTarget;}
    ID2D1SolidColorBrush* getD2DSolidColorBrush() const { return m_colorBrush;}    

    virtual TwSize<int> size() const { return m_size;}
    virtual void resize(const TwSize<int>& sz);

    virtual void translate(const TwPoint<int>& offset);
    virtual void rotate(float angle);
    virtual void scale(float factorX, float factorY);
    virtual void shear(float sx, float sy);

    virtual void pushClipRect(const TwRect<int>& rc);
    virtual void popClipRect();

    virtual void setBrushColor(const TwColor& color);
    virtual TwColor brushColor() const ;

    virtual void saveState();
    virtual void restoreState();

    virtual void beginRender();
    virtual void endRender();

    virtual void clear(const TwColor& color);

    virtual void drawLine(const TwPoint<int>& pt1, const TwPoint<int>& pt2, const TwColor& color, bool adjustBorderLine = false);

    virtual void drawRect(const TwRect<int>& rc, const TwColor& color, float width = 1.0, bool adjustBorderLine = false);
    virtual void fillRect(const TwRect<int>& rc, const TwColor& color);

    virtual void drawRoundedRect(const TwRect<int>& rc, int xradius, int yradius, const TwColor& color, float width = 1.0, bool adjustBorderLine = false);
    virtual void fillRoundedRect(const TwRect<int>& rc, int xradius, int yradius, const TwColor& color);

    virtual void drawText(const std::wstring& text, const TwRect<int>& rc );
    virtual void drawText(const std::wstring& text, const TwRect<int>& rc, const TwColor& color );
    virtual void drawText(const std::wstring& text, const TwRect<int>& rc, const TwFont& font);

    virtual void drawImage(const TwImage& image, const TwPoint<int>& pt);
    virtual void drawImage(const TwImage& image, const TwRect<int>& destRc);
    virtual void drawImage(const TwImage& image, const TwRect<int>& destRc, const TwRect<int>& srcRc);

protected:
    virtual bool init();

protected:
    TwWindow* m_window;
    bool m_init;
    TwSize<int> m_size;

    WinComPtr<ID2D1HwndRenderTarget> m_renderTarget;
    WinComPtr<ID2D1SolidColorBrush> m_colorBrush;

    WinComPtr<IDWriteTextFormat> m_textFormat;

    TwAffineTransform m_transform;

    friend class D2dSavedState;
    std::deque<std::shared_ptr<D2dSavedState> > m_savedStates;

    std::shared_ptr<D2dSavedState> m_currentState;

};
