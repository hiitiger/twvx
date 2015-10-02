#pragma once

class TW_GUI_API TwRendererResourceProvider 
{
public:
    TwRendererResourceProvider();
    ~TwRendererResourceProvider();

    WinComPtr<ID2D1Factory> getD2DFactory();

    WinComPtr<IDWriteFactory> getDWriteFactory();

    WinComPtr<IDWriteTextFormat> getTextFormat(const TwFont& font);

    TwRefPtr<TwTextMeasure> getTextMeasure(const TwFont& font);

protected:
    TwRefPtr<TwTextMeasure> createTextmeasure(const TwFont& font);

private:
    WinComPtr<ID2D1Factory> m_D2DFactory ; // Direct2D factory
    WinComPtr<IDWriteFactory> m_DWriteFactory;

    std::map<TwFont, TwRefPtr<TwTextMeasure>> m_textMeasures;
};
