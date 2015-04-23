#pragma once
#include <wincodec.h>

class TW_GUI_API WindowsImageData : public TwRefObject<TwRefBase>
{
public:
    explicit WindowsImageData(WinComPtr<IWICBitmapDecoder>& bitmapDecoder);
    ~WindowsImageData();
    void initFrames();

    TwSize<int> size();

    int frameCount() const;

    WinComPtr<IWICBitmap> bitmapAtFrame(int frame);

private:
    WinComPtr<IWICBitmapDecoder> m_bitmapDecoder;
    std::vector<WinComPtr<IWICBitmap> > m_bitmaps;
};

class TwWICImagingFactory
{
public:
    TwWICImagingFactory();
    ~TwWICImagingFactory();

    TwRefPtr<WindowsImageData> createImageData(const std::wstring& file);
    TwRefPtr<WindowsImageData> createImageData(void* data, int size);

    WinComPtr<IWICImagingFactory> getWICImagingFacotry();

private:
    WinComPtr<IWICImagingFactory> m_WICImagingacotry;
    std::map < std::wstring, TwRefPtr<WindowsImageData> > m_imageDatas;
};
