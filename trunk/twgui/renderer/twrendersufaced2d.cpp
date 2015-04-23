
#include "stable.h"
#include "twrendersufaced2d.h"

#pragma comment(lib,"Dwrite.lib")
#pragma comment(lib,"d2d1.lib")

//////////////////////////////////////////////////////////////////////////

class D2dSavedState
{
public:
    D2dSavedState(TwD2DRenderSurface* o)
        : owner(o)
        , ClipCount(0)
    {
        savedColor = owner->m_colorBrush->GetColor() ;
        savedTransform = owner->m_transform;
    }
    ~D2dSavedState()
    {
        clearClipRect();
        owner->m_colorBrush->SetColor(savedColor);
        owner->m_transform = savedTransform;
        owner->m_renderTarget->SetTransform(twMatrixFormToD2d1Matrix(savedTransform.m_matrix));
    }

    void setBrushColor(const TwColor& c)
    {
        owner->m_colorBrush->SetColor(twColorToD2D1ColorF(c));
    }

    void pushClipRect(const TwRect<int>& clip)
    {
        owner->m_renderTarget->PushAxisAlignedClip(twRectToD2D1RectF(clip), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
        ClipCount += 1;
    }

    void popClipRect()
    {
        owner->m_renderTarget->PopAxisAlignedClip();
        ClipCount -= 1;
    }

    void clearClipRect()
    {
        while (ClipCount != 0)
        {
            popClipRect();
        }
    }

    TwD2DRenderSurface* owner;
    D2D1_COLOR_F savedColor;
    int ClipCount;
    TwAffineTransform savedTransform;
};


//////////////////////////////////////////////////////////////////////////
TwD2DRenderSurface::TwD2DRenderSurface( TwWindow* window )
    : ITwRenderSurface()
    , m_window(window)
    , m_init(false)
{
    m_transform.setIdentity();
    m_init = init();
}

TwD2DRenderSurface::~TwD2DRenderSurface()
{

}

bool TwD2DRenderSurface::init()
{
    bool ret = false;
    do 
    {
        HRESULT hr = twApp()->rendererResourceProvider()->getD2DFactory()->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties( (HWND)(m_window->windowId()), twSizeToD2D1SizeU(m_window->clientRect().size())),
            m_renderTarget.resetAndGetPointerAddress()
            );
        ret = SUCCEEDED(hr) && m_renderTarget;

        if (!ret)
        {
            break;
        }

        hr = m_renderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF(D2D1::ColorF::Black)),
            m_colorBrush.resetAndGetPointerAddress()
            );
        ret = SUCCEEDED(hr) && m_renderTarget;

        if (!ret)
        {
            break;
        }

        hr = twApp()->rendererResourceProvider()->getDWriteFactory()->CreateTextFormat(
            L"Microsoft YaHei UI",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            16.0f,
            L"",
            m_textFormat.resetAndGetPointerAddress()
            );
        ret = SUCCEEDED(hr) && m_textFormat;

        if (!ret)
        {
            break;
        }

        m_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

    } while (false);

    return ret;
}

void TwD2DRenderSurface::resize( const TwSize<int>& sz )
{
    m_size = sz;
    m_renderTarget->Resize(D2D1::SizeU(m_size.width(), m_size.height()));
}

void TwD2DRenderSurface::translate( const TwPoint<int>&  offset)
{
    m_transform.translate((float)offset.x(), (float)offset.y());
    m_renderTarget->SetTransform(twMatrixFormToD2d1Matrix(m_transform.m_matrix));
}

void TwD2DRenderSurface::rotate(float angle)
{
    m_transform.rotate(angle);
    m_renderTarget->SetTransform(twMatrixFormToD2d1Matrix(m_transform.m_matrix));
}

void TwD2DRenderSurface::scale(float factorX, float factorY)
{
    m_transform.scale(factorX, factorY);
    m_renderTarget->SetTransform(twMatrixFormToD2d1Matrix(m_transform.m_matrix));
}

void TwD2DRenderSurface::shear(float sx, float sy)
{
    m_transform.shear(sx, sy);
    m_renderTarget->SetTransform(twMatrixFormToD2d1Matrix(m_transform.m_matrix));
}

void TwD2DRenderSurface::pushClipRect( const TwRect<int>&  clip)
{
    m_currentState->pushClipRect(clip);
}

void TwD2DRenderSurface::popClipRect()
{
    m_currentState->popClipRect();
}

void TwD2DRenderSurface::setBrushColor( const TwColor& color )
{
    m_colorBrush->SetColor(twColorToD2D1ColorF(color));
}

TwColor TwD2DRenderSurface::brushColor() const
{
    return D2D1ColorFToTwColor(m_colorBrush->GetColor());
}

void TwD2DRenderSurface::saveState()
{
    std::shared_ptr<D2dSavedState> saved(new D2dSavedState(this));
    m_savedStates.push_back(saved);
    m_currentState = saved;
}

void TwD2DRenderSurface::restoreState()
{
    DAssert(m_savedStates.size() > 1);
    m_savedStates.pop_back();
    m_currentState = m_savedStates.back();
}

void TwD2DRenderSurface::beginRender()
{
    m_renderTarget->BeginDraw();
    m_renderTarget->SetTransform(twMatrixFormToD2d1Matrix(m_transform.m_matrix));
    m_renderTarget->Clear(m_colorBrush->GetColor());
    saveState();
}

void TwD2DRenderSurface::endRender()
{
    m_savedStates.clear();
    m_currentState.reset();
    m_renderTarget->EndDraw();
}

void TwD2DRenderSurface::clear( const TwColor& color )
{
    m_renderTarget->Clear(D2D1::ColorF(color.red(), color.green(), color.blue(), color.alpha()));
}

void TwD2DRenderSurface::drawLine(const TwPoint<int>& pt1, const TwPoint<int>& pt2, const TwColor& color, bool adjustBorderLine)
{
    D2D1_COLOR_F savedColor = m_colorBrush->GetColor();
    m_colorBrush->SetColor(twColorToD2D1ColorF(color));

    if (adjustBorderLine)
    {
        TwPoint<float> ptAdjust1(pt1.x() + 0.5f, pt1.y() + 0.5f);
        TwPoint<float> ptAdjust2(pt2.x() - 0.5f, pt2.y() - 0.5f);
        m_renderTarget->DrawLine(twPointToD2D1PointF(ptAdjust1), twPointToD2D1PointF(ptAdjust2), m_colorBrush);
    }
    else
    {
        m_renderTarget->DrawLine(twPointToD2D1PointF(pt1), twPointToD2D1PointF(pt2), m_colorBrush);
    }

    m_colorBrush->SetColor(savedColor);
}

void TwD2DRenderSurface::drawRect(const TwRect<int>& rc, const TwColor& color, float width, bool adjustBorderLine)
{
    D2D1_COLOR_F savedColor = m_colorBrush->GetColor();
    m_colorBrush->SetColor(twColorToD2D1ColorF(color));

    if (adjustBorderLine)
    {
        TwRect<float> rcAdjust(rc.left() + 0.5f, rc.top() + 0.5f, rc.right() - 0.5f, rc.bottom() - 0.5f);
        m_renderTarget->DrawRectangle(twRectToD2D1RectF(rcAdjust), m_colorBrush, width);
    }
    else
    {
        m_renderTarget->DrawRectangle(twRectToD2D1RectF(rc), m_colorBrush, width);
    }

    m_colorBrush->SetColor(savedColor);
}

void TwD2DRenderSurface::fillRect( const TwRect<int>& rc, const TwColor& color )
{
    D2D1_COLOR_F savedColor = m_colorBrush->GetColor();
    m_colorBrush->SetColor(twColorToD2D1ColorF(color));

    m_renderTarget->FillRectangle(twRectToD2D1RectF(rc), m_colorBrush);

    m_colorBrush->SetColor(savedColor);

}

void TwD2DRenderSurface::drawRoundedRect(const TwRect<int>& rc, int xradius, int yradius, const TwColor& color, float width, bool adjustBorderLine)
{
    D2D1_COLOR_F savedColor = m_colorBrush->GetColor();
    m_colorBrush->SetColor(twColorToD2D1ColorF(color));

    D2D1_ROUNDED_RECT roundedRect;
    if (adjustBorderLine)
    {
        TwRect<float> rcAdjust(rc.left() + 0.5f, rc.top() + 0.5f, rc.right() - 0.5f, rc.bottom() - 0.5f);
        roundedRect = D2D1::RoundedRect(
            twRectToD2D1RectF(rcAdjust),
            static_cast<float>(xradius),
            static_cast<float>(yradius)
            );
    }
    else
    {
        roundedRect = D2D1::RoundedRect(
            twRectToD2D1RectF(rc),
            static_cast<float>(xradius),
            static_cast<float>(yradius)
            );
    }

    m_renderTarget->DrawRoundedRectangle(&roundedRect, m_colorBrush, width);

    m_colorBrush->SetColor(savedColor);
}

void TwD2DRenderSurface::fillRoundedRect( const TwRect<int>& rc, int xradius, int yradius, const TwColor& color )
{
    D2D1_COLOR_F savedColor = m_colorBrush->GetColor();
    m_colorBrush->SetColor(twColorToD2D1ColorF(color));

    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
        twRectToD2D1RectF(rc),
        static_cast<float>(xradius),
        static_cast<float>(yradius)
        );
    m_renderTarget->FillRoundedRectangle(&roundedRect, m_colorBrush);

    m_colorBrush->SetColor(savedColor);
}


void TwD2DRenderSurface::drawText( const std::wstring& text, const TwRect<int>& rc )
{
    m_renderTarget->DrawTextW(text.c_str(), text.length(), m_textFormat, twRectToD2D1RectF(rc), m_colorBrush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
}

void TwD2DRenderSurface::drawText( const std::wstring& text, const TwRect<int>& rc, const TwColor& color )
{
    D2D1_COLOR_F savedColor = m_colorBrush->GetColor();
    m_colorBrush->SetColor(twColorToD2D1ColorF(color));

    m_renderTarget->DrawTextW(text.c_str(), text.length(), m_textFormat, twRectToD2D1RectF(rc), m_colorBrush, D2D1_DRAW_TEXT_OPTIONS_CLIP);

    m_colorBrush->SetColor(savedColor);
}

void TwD2DRenderSurface::drawText(const std::wstring& text, const TwRect<int>& rc, const TwFont& font)
{
    WinComPtr<IDWriteTextFormat> textFormat;

    HRESULT hr = twApp()->rendererResourceProvider()->getDWriteFactory()->CreateTextFormat(
        font.m_fontFamily.c_str(),
        NULL,
        font.m_bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
        font.m_italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        font.m_size,
        L"",
        textFormat.resetAndGetPointerAddress()
        );

    if (SUCCEEDED(hr))
    {
        textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

        WinComPtr<IDWriteTextLayout> textLayout;

        hr = twApp()->rendererResourceProvider()->getDWriteFactory()->CreateTextLayout(
            text.c_str(),
            text.length(),
            textFormat,
            0,
            0,
            textLayout.resetAndGetPointerAddress());

        if (SUCCEEDED(hr))
        {
            if (font.m_underline)
            {
                DWRITE_TEXT_RANGE textRange;
                textRange.startPosition = 0;
                textRange.length = (int)text.length();
                textLayout->SetUnderline(TRUE, textRange);
            }

            if (font.m_strikeline)
            {
                DWRITE_TEXT_RANGE textRange;
                textRange.startPosition = 0;
                textRange.length = (int)text.length();
                textLayout->SetStrikethrough(TRUE, textRange);
            }

            textLayout->SetMaxHeight((float)rc.height());
            textLayout->SetMaxWidth((float)rc.width());

            m_renderTarget->DrawTextLayout(
                twPointToD2D1PointF(rc.topLeft()),
                textLayout,
                m_colorBrush,
                D2D1_DRAW_TEXT_OPTIONS_NO_SNAP | D2D1_DRAW_TEXT_OPTIONS_CLIP
                );
        }
    }
}

void TwD2DRenderSurface::drawImage(const TwImage& image, const TwPoint<int>& pt)
{
    TwRect<int> destRc(pt, image.size());
    drawImage(image, destRc);
}

void TwD2DRenderSurface::drawImage(const TwImage& image, const TwRect<int>& destRc)
{
    TwRefPtr<WindowsImageData> imageData = image.imageData();

    WinComPtr<ID2D1Bitmap> D2D1Bitmap;
    m_renderTarget->CreateBitmapFromWicBitmap(imageData->bitmapAtFrame(image.currentFrame()), D2D1Bitmap.resetAndGetPointerAddress());

    m_renderTarget->DrawBitmap(D2D1Bitmap, twRectToD2D1RectF(destRc), 1.f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}

void TwD2DRenderSurface::drawImage(const TwImage& image, const TwRect<int>& destRc, const TwRect<int>& srcRc)
{
    TwRefPtr<WindowsImageData> imageData = image.imageData();

    WinComPtr<ID2D1Bitmap> D2D1Bitmap;
    m_renderTarget->CreateBitmapFromWicBitmap(imageData->bitmapAtFrame(image.currentFrame()), D2D1Bitmap.resetAndGetPointerAddress());

    m_renderTarget->DrawBitmap(D2D1Bitmap, twRectToD2D1RectF(destRc), 1.f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, twRectToD2D1RectF(srcRc));
}

