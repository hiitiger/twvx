#include "stable.h"
#include "twsingleton.h"
#include "twimagedata.h"

#pragma comment(lib,"WindowsCodecs.lib")

WindowsImageData::WindowsImageData(WinComPtr<IWICBitmapDecoder>& bitmapDecoder)
    : m_bitmapDecoder(bitmapDecoder)
{
    initFrames();
}

WindowsImageData::~WindowsImageData()
{

}

void WindowsImageData::initFrames()
{
    UINT frames;
    m_bitmapDecoder->GetFrameCount(&frames);
    if (frames < 1)
    {
        return;
    }
    m_bitmaps.resize(frames);
}

TwSize<int> WindowsImageData::size()
{
    if (frameCount() > 0)
    {
        UINT width = 0;
        UINT height = 0;
        bitmapAtFrame(0)->GetSize(&width, &height);
        return TwSize<int>(width, height);
    }
    else
    {
        return TwSize<int>();
    }
}

int WindowsImageData::frameCount() const
{
    return m_bitmaps.size();
}

WinComPtr<IWICBitmap> WindowsImageData::bitmapAtFrame(int frame)
{
    if (frame >= 0 && frame < frameCount())
    {
        WinComPtr<IWICBitmap>& bitmap = m_bitmaps[frame];
        if (bitmap)
        {
            return bitmap;
        }
        else
        {
            WinComPtr<IWICBitmapFrameDecode> frameDecode;
            HRESULT hr = m_bitmapDecoder->GetFrame(0, frameDecode.resetAndGetPointerAddress());

            if (FAILED(hr))
            {
                return nullptr;
            }

            WinComPtr<IWICFormatConverter> formatConverter = 0;
            hr = twApp()->imagingFactory()->getWICImagingFacotry()->CreateFormatConverter(formatConverter.resetAndGetPointerAddress());
            if (FAILED(hr))
            {
                return nullptr;
            }

            hr = formatConverter->Initialize(
                frameDecode,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
                );
            if (FAILED(hr))
            {
                return nullptr;
            }

            hr = twApp()->imagingFactory()->getWICImagingFacotry()->CreateBitmapFromSource(formatConverter, WICBitmapCacheOnLoad, bitmap.resetAndGetPointerAddress());
            if (FAILED(hr))
            {
                return nullptr;
            }

            return bitmap;
        }
    }
    return nullptr;
}

//////////////////////////////////////////////////////////////////////////
TwWICImagingFactory::TwWICImagingFactory()
{
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        (LPVOID*)m_WICImagingacotry.resetAndGetPointerAddress()
        );
    if (FAILED(hr))
    {
    }
}

TwWICImagingFactory::~TwWICImagingFactory()
{

}

WinComPtr<IWICImagingFactory> TwWICImagingFactory::getWICImagingFacotry()
{
    return m_WICImagingacotry;
}

TwRefPtr<WindowsImageData> TwWICImagingFactory::createImageData(const std::wstring& file)
{
    auto it = m_imageDatas.find(file);
    if (it != m_imageDatas.end())
    {
        return it->second;
    }

    WinComPtr<IWICBitmapDecoder> bitmapDecoder = NULL;
    HRESULT hr = m_WICImagingacotry->CreateDecoderFromFilename(
        file.c_str(),
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        bitmapDecoder.resetAndGetPointerAddress()
        );
    if (FAILED(hr))
    {
        return nullptr;
    }

    TwRefPtr<WindowsImageData> newImageData = new WindowsImageData(bitmapDecoder);
    m_imageDatas.insert(std::make_pair(file, newImageData));

    return newImageData;
}

TwRefPtr<WindowsImageData> TwWICImagingFactory::createImageData(void* data, int size)
{
    WinComPtr<IWICStream> stream = NULL;
    HRESULT hr = m_WICImagingacotry->CreateStream(stream.resetAndGetPointerAddress());
    if (FAILED(hr))
    {
        return nullptr;
    }

    hr = stream->InitializeFromMemory(
        reinterpret_cast<BYTE*>(data),
        size
        );
    if (FAILED(hr))
    {
        return nullptr;
    }

    WinComPtr<IWICBitmapDecoder> bitmapDecoder = NULL;
    hr = m_WICImagingacotry->CreateDecoderFromStream(
        stream,
        NULL,
        WICDecodeMetadataCacheOnLoad,
        bitmapDecoder.resetAndGetPointerAddress()
        );

    if (FAILED(hr))
    {
        return nullptr;
    }

    TwRefPtr<WindowsImageData> newImageData = new WindowsImageData(bitmapDecoder);

    return newImageData;
}


