#pragma once
#include "twlayout.h"

class TwBoxLayout : public TwLayoutManager
{
public:
    TwBoxLayout(TwObject* parent, Tw::Orientation o);
    ~TwBoxLayout();

    virtual void onWidgetAdd(TwWidget* child);
    virtual void onWidgetRemove(TwWidget* child);

protected:
    virtual void layoutWidgets();
    virtual void setupGeometry();

private:
    Tw::Orientation m_orientation;
};