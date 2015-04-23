#pragma once

class TW_GUI_API TwImage
{
public:
    TwImage();
    explicit TwImage(const std::wstring& file);

    TwSize<int> size() const;
    int width();
    int height();

    int frameCount() const;
    int currentFrame() const;
    void setCurrentFrame(int);

    TwRefPtr<WindowsImageData> imageData() const { return m_imageData; }

private:
    TwRefPtr<WindowsImageData> m_imageData;
    int m_currentFrame;
};