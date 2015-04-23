#include "stable.h"
#include "twrender.h"

TwSimpleTextRenderer::TwSimpleTextRenderer()
    : m_D2DSurface(nullptr)
    , m_halign(Tw::Align_Center)
    , m_valign(Tw::Align_Center)
    , m_autoMultiLine(false)
{

}

TwSimpleTextRenderer::~TwSimpleTextRenderer()
{
    m_D2DSurface = nullptr;
    m_textFormat = nullptr;
    m_textLayout = nullptr;
}

void TwSimpleTextRenderer::setRenderSurface( ITwRenderSurface* renderSurface )
{
    m_D2DSurface = dynamic_cast<TwD2DRenderSurface*>(renderSurface);   
}

void TwSimpleTextRenderer::render( TwRect<int> rc )
{
    if (!rc.isValid())
    {
        return;
    }

    if (!m_textLayout)
    {
        return;
    }

    m_textLayout->SetMaxHeight((float)rc.height());
    m_textLayout->SetMaxWidth((float)rc.width());

    ID2D1SolidColorBrush* brush = m_D2DSurface->getD2DSolidColorBrush();
    D2D1_COLOR_F savedColor = brush->GetColor();
    brush->SetColor(twColorToD2D1ColorF(m_color));

    m_D2DSurface->getD2DRenderTarget()->DrawTextLayout(
        twPointToD2D1PointF(rc.topLeft()),
        m_textLayout,
        brush,
        D2D1_DRAW_TEXT_OPTIONS_NO_SNAP | D2D1_DRAW_TEXT_OPTIONS_CLIP  
        );

    brush->SetColor(savedColor);
}

void TwSimpleTextRenderer::updateRenderer()
{
    m_textFormat = nullptr;
    m_textLayout = nullptr;
    createTextFormat();
    createTextLayout();
}


void TwSimpleTextRenderer::createTextFormat()
{
    m_textFormat = twApp()->rendererResourceProvider()->getTextFormat(m_font);

    if (m_textFormat)
    {

        m_textFormat->SetWordWrapping(m_autoMultiLine ?DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);

        switch (m_halign)
        {
        case Tw::Align_Left:
            m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
            break;
        case Tw::Align_Center:
            m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            break;    
        case Tw::Align_Right:
            m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
            break;
        }

        switch (m_valign)
        {
        case Tw::Align_Center:
            m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            break;
        case Tw::Align_Top:
            m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            break;
        case Tw::Align_Bottom:
            m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
            break;
        }
    }
}

void TwSimpleTextRenderer::createTextLayout()
{
    if (m_textFormat)
    {
        HRESULT hr = twApp()->rendererResourceProvider()->getDWriteFactory()->CreateTextLayout(
            m_text.c_str(),
            m_text.length(),
            m_textFormat,
            0,
            0,
            m_textLayout.resetAndGetPointerAddress());

        if (SUCCEEDED(hr))
        {
            if (m_font.m_underline)
            {
                DWRITE_TEXT_RANGE textRange;
                textRange.startPosition = 0;
                textRange.length = (int)m_text.length();
                m_textLayout->SetUnderline(TRUE, textRange);
            }
 
            if (m_font.m_strikeline)
            {
                DWRITE_TEXT_RANGE textRange;
                textRange.startPosition = 0;
                textRange.length = (int)m_text.length();
                m_textLayout->SetStrikethrough(TRUE, textRange);
            }

            DWRITE_TEXT_METRICS metrics;
            HRESULT hr = m_textLayout->GetMetrics(&metrics);
            if (SUCCEEDED(hr))
            {
                m_minSize.setWidth((int)ceil(metrics.widthIncludingTrailingWhitespace));
                m_minSize.setHeight((int)ceil(metrics.height));
            }

            WinComPtr<IDWriteInlineObject> ellipseInlineObject = 0;
            twApp()->rendererResourceProvider()->getDWriteFactory()->CreateEllipsisTrimmingSign(m_textFormat, ellipseInlineObject.resetAndGetPointerAddress());
            DWRITE_TRIMMING trimming;
            trimming.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
            trimming.delimiter = 0;
            trimming.delimiterCount = 0;
            m_textLayout->SetTrimming(&trimming, ellipseInlineObject);
        }
    }
}

TwSize<int> TwSimpleTextRenderer::minSize() const
{
    return m_minSize;
}
