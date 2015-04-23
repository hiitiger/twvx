#include "stable.h"
#include "twpainter.h"

bool TwPainter::initRenderSys()
{
    ::CoInitialize(nullptr);

    return true;
}

void TwPainter::unitRenderSys()
{
    ::CoUninitialize();
}

TwPainter::TwPainter( ITwRenderSurface* surface )
    : m_surface(surface)
{

}

TwPainter::~TwPainter()
{

}

void TwPainter::translate( const TwPoint<int>& offset )
{
    m_surface->translate(offset);
}

void TwPainter::translate( int x, int y )
{
    m_surface->translate(TwPoint<int>(x, y));
}

void TwPainter::rotate(float angle)
{
    m_surface->rotate(angle);
}

void TwPainter::scale(float factorX, float factorY)
{
    m_surface->scale(factorX, factorY);
}

void TwPainter::shear(float sx, float sy)
{
    m_surface->shear(sx, sy);
}

void TwPainter::pushClipRect( const TwRect<int>& rc )
{
    m_clips.push_back(rc);
    m_clipCounter.back() = m_clipCounter.back() + 1;
    m_surface->pushClipRect(rc);
}

void TwPainter::popClipRect()
{
    m_clips.pop_back();
    m_clipCounter.back() = m_clipCounter.back() - 1;
    m_surface->popClipRect();
}

TwRect<int> TwPainter::lastPushedClipRect()
{
    if (!m_clips.empty())
    {
        return m_clips.back();
    }
    return TwRect<int>();
}

void TwPainter::setBrushColor( const TwColor& color )
{
    m_surface->setBrushColor(color);
}

TwColor TwPainter::brushColor() const
{
    return m_surface->brushColor();
}

void TwPainter::saveState()
{
    m_clipCounter.push_back(0);
    m_surface->saveState();
}

void TwPainter::restoreState()
{
    int clips = m_clipCounter.back();
    m_clipCounter.pop_back();
    while (clips != 0)
    {
        m_clips.pop_back();
        clips -= 1;
    }
    m_surface->restoreState();
}

void TwPainter::beginRender()
{
    m_clips.clear();
    m_clipCounter.clear();
    m_surface->beginRender();
}

void TwPainter::endRender()
{
    m_clips.clear();
    m_clipCounter.clear();
    m_surface->endRender();
}

void TwPainter::beginLayer( float /*transparency*/, const TwRect<int>& /*bounds*/ )
{
    //TODO
    DAssert_M(false, "not implement");
}

void TwPainter::endLayer()
{
    //TODO
}

void TwPainter::paint( ITwRenderer* renderer, const TwRect<int>& rc )
{
    renderer->render(rc);
}

void TwPainter::drawLine(const TwPoint<int>& pt1, const TwPoint<int>& pt2, const TwColor& color, bool adjustBorderLine )
{
    //
    m_surface->drawLine(pt1, pt2, color, adjustBorderLine);

}

void TwPainter::drawRect(const TwRect<int>& rc, const TwColor& color, float width , bool adjustBorderLine)
{
    //
    m_surface->drawRect(rc, color, width, adjustBorderLine);
}

void TwPainter::fillRect( const TwRect<int>& rc, const TwColor& color)
{
    //
    m_surface->fillRect(rc, color);
}

void TwPainter::drawRoundedRect(const TwRect<int>& rc, int xradius, int yradius, const TwColor& color, float width , bool adjustBorderLine)
{
    m_surface->drawRoundedRect(rc, xradius, yradius, color, width, adjustBorderLine);
}

void TwPainter::fillRoundedRect( const TwRect<int>& rc, int xradius, int yradius, const TwColor& color )
{
    m_surface->fillRoundedRect(rc,xradius, yradius, color);
}

void TwPainter::drawText( const std::wstring& text, const TwRect<int>& rc )
{
    //
    m_surface->drawText(text, rc);
}

void TwPainter::drawText( const std::wstring& text, const TwRect<int>& rc, const TwColor& color )
{
    m_surface->drawText(text, rc, color);
}

void TwPainter::drawText(const std::wstring& text, const TwRect<int>& rc, const TwFont& font)
{
    m_surface->drawText(text, rc, font);
}

void TwPainter::drawImage(const TwImage& image, const TwPoint<int>& pt)
{
    m_surface->drawImage(image, pt);
}

void TwPainter::drawImage(const TwImage& image, const TwRect<int>& destRc)
{
    m_surface->drawImage(image, destRc);
}

void TwPainter::drawImage(const TwImage& image, const TwRect<int>& destRc, const TwRect<int>& srcRc)
{
    m_surface->drawImage(image, destRc, srcRc);
}

