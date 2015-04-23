#include "stable.h"
#include "twrenderresourceprovider.h"

#include "twtextmeasure.h"

class TwTextMeasureD2D : public TwTextMeasure
{
public:
    TwTextMeasureD2D(WinComPtr<IDWriteTextFormat>& textFormat)
        : m_textFormat(textFormat)
    {

    }
protected:
    virtual TwSize<int> measureTextInternal(wchar_t c)
    {
        TwSize<int> size(0, 0);
        WinComPtr<IDWriteTextLayout> textLayout;
        HRESULT hr = twApp()->rendererResourceProvider()->getDWriteFactory()->CreateTextLayout(&c, 1, m_textFormat, 0, 0, textLayout.resetAndGetPointerAddress());
        if (SUCCEEDED(hr))
        {
            DWRITE_TEXT_METRICS metrics;
            HRESULT hr = textLayout->GetMetrics(&metrics);
            if (SUCCEEDED(hr))
            {
                size.setWidth((int)ceil(metrics.widthIncludingTrailingWhitespace));
                size.setHeight((int)ceil(metrics.height));
            }
        }
        return size;
    }
    virtual TwSize<int> measureTextInternal(const std::wstring& text)
    {
        TwSize<int> size(0, 0);
        WinComPtr<IDWriteTextLayout> textLayout;
        HRESULT hr = twApp()->rendererResourceProvider()->getDWriteFactory()->CreateTextLayout(text.c_str(), text.length(), m_textFormat, 0, 0, textLayout.resetAndGetPointerAddress());
        if (SUCCEEDED(hr))
        {
            DWRITE_TEXT_METRICS metrics;
            HRESULT hr = textLayout->GetMetrics(&metrics);
            if (SUCCEEDED(hr))
            {
                size.setWidth((int)ceil(metrics.widthIncludingTrailingWhitespace));
                size.setHeight((int)ceil(metrics.height));
            }
        }
        return size;
    }

private:
    WinComPtr<IDWriteTextFormat> m_textFormat;
};

TwRendererResourceProvider::TwRendererResourceProvider()
{
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        m_D2DFactory.resetAndGetPointerAddress()
        );

   if (FAILED(hr))
   {
   }

   hr = DWriteCreateFactory(
       DWRITE_FACTORY_TYPE_SHARED,
       __uuidof(m_DWriteFactory),
       reinterpret_cast<IUnknown **>(m_DWriteFactory.resetAndGetPointerAddress())
       );

   if (FAILED(hr))
   {
   }
}

TwRendererResourceProvider::~TwRendererResourceProvider()
{

}

WinComPtr<ID2D1Factory> TwRendererResourceProvider::getD2DFactory()
{
    return m_D2DFactory;
}

WinComPtr<IDWriteFactory> TwRendererResourceProvider::getDWriteFactory()
{
    return m_DWriteFactory;
}


WinComPtr<IDWriteTextFormat> TwRendererResourceProvider::getTextFormat(const TwFont& font)
{
    WinComPtr<IDWriteTextFormat> textFormat;
    m_DWriteFactory->CreateTextFormat(font.m_fontFamily.c_str(),
        NULL,
        font.m_bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
        font.m_italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        font.m_size,
        L"",
        textFormat.resetAndGetPointerAddress()
        );

    return textFormat;
}

TwRefPtr<TwTextMeasure> TwRendererResourceProvider::getTextMeasure(const TwFont& font)
{
    auto it = m_textMeasures.find(font);
    if (it != m_textMeasures.end())
    {
        return it->second;
    }
    else
    {
        TwRefPtr<TwTextMeasure> measure = createTextmeasure(font);
        m_textMeasures.insert(std::make_pair(font, measure));
        return measure;
    }
}

TwRefPtr<TwTextMeasure> TwRendererResourceProvider::createTextmeasure(const TwFont& font)
{
    TwTextMeasureD2D* textMeasure = new TwTextMeasureD2D(getTextFormat(font));

    return TwRefPtr<TwTextMeasure>(textMeasure);
}


//////////////////////////////////////////////////////////////////////////
