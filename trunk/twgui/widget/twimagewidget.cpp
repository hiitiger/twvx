#include "stable.h"
#include "twimagewidget.h"

TW_CLASS_IMPLEMENT(TwImageWidget, TwWidget);

TwImageWidget::TwImageWidget(TwWidget* parent /*= nullptr*/)
    : TwWidget(parent)
{

}

TwImageWidget::~TwImageWidget()
{

}

void TwImageWidget::setImage(const TwImage& image)
{
    m_image = image;
    updateBound();
}

TwSize<int> TwImageWidget::hintSize() const
{
    return m_image.size();
}

void TwImageWidget::paint(TwPainter* painter)
{
    painter->drawImage(m_image, calcImageRect());
}

TwRect<int> TwImageWidget::calcImageRect()
{
    TwSize<int> imageSize = m_image.size();

    int x = (width() - imageSize.width()) / 2;
    int y = (height() - imageSize.height()) / 2;
    TwRect<int> imageRect(TwPoint<int>(x, y), imageSize);
    return imageRect;
}
