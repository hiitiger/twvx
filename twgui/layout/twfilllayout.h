#pragma once
#include "twlayout.h"

class TW_GUI_API TwFillLayout : public TwLayoutManager
{
public:
    explicit TwFillLayout(TwObject* parent);
    ~TwFillLayout();

    virtual void onWidgetAdd(TwWidget* child);
    virtual void onWidgetRemove(TwWidget* child);

protected:
    virtual void layoutWidgets();
    virtual void setupGeometry();

};