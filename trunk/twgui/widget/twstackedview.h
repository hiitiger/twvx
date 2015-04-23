#pragma once

class TW_GUI_API TwStackedView : public TwWidget
{
    TW_CLASS_DECLARE(TwStackedView);

public:
    explicit TwStackedView(TwWidget* parent = nullptr);
    ~TwStackedView();

};

TW_CLASS_REGISTER(TwStackedView);
