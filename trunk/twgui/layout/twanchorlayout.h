#pragma once
#include "twlayout.h"

class TwAnchorLayout : public TwLayoutManager
{
public:
    explicit TwAnchorLayout(TwObject* parent);
    ~TwAnchorLayout();

    virtual void onWidgetAdd(TwWidget* child);
    virtual void onWidgetRemove(TwWidget* child);

protected:
    virtual void layoutWidgets();
    virtual void setupGeometry();

};