#include "stable.h"
#include "twimagedata.h"
#include "twimage.h"

TwImage::TwImage()
    : m_currentFrame(-1)
{

}

TwImage::TwImage(const std::wstring& file)
    : m_currentFrame(-1)
{
    m_imageData = twApp()->imagingFactory()->createImageData(file);
    if (m_imageData && frameCount() >= 0)
    {
        m_currentFrame = 0;
    }
}

TwSize<int> TwImage::size() const
{
    if (imageData())
    {
        return imageData()->size();
    }
    else
    {
        return TwSize<int>();
    }
}

int TwImage::width()
{
    return size().width();
}

int TwImage::height()
{
    return size().height();
}

int TwImage::frameCount() const
{
    if (m_imageData)
    {
        return m_imageData->frameCount();
    }
    return 0;
}

int TwImage::currentFrame() const
{
    return m_currentFrame;
}

void TwImage::setCurrentFrame(int frame)
{
    DAssert(frame >= 0 && frame <= frameCount());
    if (frame >= 0 && frame <= frameCount())
    {
        m_currentFrame = frame;
    }
}
