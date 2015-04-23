#pragma once

class TW_GUI_API TwLayoutManager : public TwObject
{
public:
    explicit TwLayoutManager(TwObject* parent = nullptr);
    virtual ~TwLayoutManager();

    void setHostWidget(TwWidget*);

    void layout();
    void activeLayout();
    void invalidateLayout();

    virtual void onWidgetAdd(TwWidget* child);
    virtual void onWidgetRemove(TwWidget* child);

    TwSize<int> preferredSize();

protected:
    virtual void layoutWidgets() = 0;

    virtual void setupGeometry();

protected:
    TwWidget* m_hostWidget;
    bool m_dirty;
    TwSize<int> m_cacheHostSize;
    TwSize<int> m_cachePerredSize;
};