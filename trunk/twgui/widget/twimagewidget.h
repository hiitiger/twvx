#pragma once

class TW_GUI_API TwImageWidget : public TwWidget
{
    TW_CLASS_DECLARE(TwImageWidget);

public:
    explicit TwImageWidget(TwWidget* parent = nullptr);
    ~TwImageWidget();

    void setImage(const TwImage&);

    virtual TwSize<int> hintSize() const;

    virtual void paint(TwPainter*);

protected:
    TwRect<int> calcImageRect();

private:
    TwImage m_image;
};

TW_CLASS_REGISTER(TwImageWidget);
