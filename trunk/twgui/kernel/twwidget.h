#pragma once
#include "twcontrol.h"

class TwLayoutManager;

/*
    TwWidget is the Gui base class

    construct order :
        object tree --> widget tree --> layout tree
    destruct :
        layout tree --> widget tree --> object tree
    
*/

class TW_GUI_API TwWidget : public TwControl
{
    TW_CLASS_DECLARE(TwWidget);
public:
    enum LayoutType
    {
        Default,
        HBox, 
        VBox,
        Grid,
        Anchor,
        Fill,
    };

    enum ChangeType
    {
        VisibleChange,
        EnableChange,
        CalcedLayoutSizeChange,
        ZOrderChange,
        AlignmentChange,
    };

    explicit TwWidget(TwWidget* parent = nullptr);
    virtual ~TwWidget();

    TwWindow* window() const;
    TwWidget* rootWidget() const { return m_rootWidget; }

    void setParentWidget(TwWidget* parentWidget);
    TwWidget* parentWidget() const { return m_parentWidget; }

    int childWidgetCount() const;
    TwWidget* childWidgetAt(int index);

    bool isAncestorOf(const TwWidget*);
    TwWidget* findChildWidget(const std::wstring& name);

    //use for layout manager to do layout
    //if need put into layout system , need call this
    void addWidget(TwWidget* w);
    void removeWidget(TwWidget* w);
    int widgetCount() const;
    TwWidget* widgetAt(int index);
    bool isWidgetAdded(TwWidget* w);

    //display
    void show();
    void hide();
    void setVisible(bool visible);
    bool isVisible() const { return m_visible; }

    void setEnabled(bool enable);
    bool isEnabled() const { return m_enabled; }

    virtual void updateBound();
    void update();

    void setFocus();
    void clearFocus();
    bool hasFocus() const;

    void setAcceptMouseButton(Tw::MouseButton button, bool accept);
    bool acceptMouseButton(Tw::MouseButton button);

    void setWidgetFlag(Tw::WidgetFlag flag, bool set);
    bool testWidgetFlag(Tw::WidgetFlag flag) const { return (m_widgetFlags & static_cast<int64>(flag)) == flag; }

    void setAcceptHoverEvent(bool accpet) { setWidgetFlag(Tw::AcceptHoverEvent, accpet); }
    bool acceptHoverEvent() const { return testWidgetFlag(Tw::AcceptHoverEvent); }

    void setAcceptDragMoveEvent(bool accpet) { setWidgetFlag(Tw::AcceptDragMoveEvent, accpet); }
    bool acceptDragMoveEvent() const { return testWidgetFlag(Tw::AcceptDragMoveEvent); }

    void setAcceptDragDropEvent(bool accpet) { setWidgetFlag(Tw::AcceptDragDropEvent, accpet); }
    bool accpetDragDropEvent() const { return testWidgetFlag(Tw::AcceptDragDropEvent); }

    //geometry
    void setPos(const TwPoint<int>& pt);
    void setPos(int x, int y);
    TwPoint<int> pos() const;
    void setX(int x);
    int x() const;
    void setY(int y);
    int y() const;

    void setSize(const TwSize<int>& sz);
    void setSize(int width, int height);
    TwSize<int> size() const;

    int width() const;
    void setWidth(int w);
    int height() const;
    void setHeight(int h);

    //setWidth, setHeight and setSize method will also set size policy to Fixed!!
    void setSizePolicy(Tw::SizePolicy widthPolicy, Tw::SizePolicy heightPolicy);

    //in parent, it's the bounds of this widget
    void setBoundRect(const TwRect<int>& rc);
    void setBoundRect(const TwPoint<int>& topLeft, const TwSize<int>& sz);
    void setBoundRect(int left, int top, int width, int height);
    TwRect<int> boundRect() const;
    //in local 
    TwRect<int> localRect() const;
    bool contains(const TwPoint<int>& localPoint);
    //in window
    TwRect<int> rectInWindow() const;
    TwRect<int> rectInRoot() const;

    void setZOrder(int z);
    int zOrder() const { return m_zOrder; }

    //layout
    void setLayout(LayoutType layoutType);
    void setLayout(TwLayoutManager*);
    TwLayoutManager* layoutmanger() const { return m_layout.get(); }

    void setContentMargin(const TwMargin<int>& margin);
    TwMargin<int> contentMargin() const;
    void setChildSpacing(int space);
    int childSpacing() const;

    void setAlignment(Tw::Alignment hAlign, Tw::Alignment vAlign );
    void setHorizontalAlignment(Tw::Alignment hAlign);
    void setVerticalAlignment(Tw::Alignment vAlign);
    Tw::Alignment horizontalAlignment() const { return m_hAlignment; }
    Tw::Alignment verticalAlignment() const { return m_vAlignment; }

    void layout();
    virtual void layoutInternalWidgets();
    virtual void layoutWidgets();

    //
    TwSize<int> calcLayoutSize() const;
    TwSize<int> preferredSize() const;
    virtual TwSize<int> hintSize() const;

    void setCursor(TwCursor);
    void clearCursor();
    TwCursor cursor() const;
    TwWidget* eventHandlerFromPoint(const TwPoint<int>& pt);

    //coordinate, arg is in local coordinate
    TwPoint<int> mapToParent(const TwPoint<int>& pt) const;
    TwPoint<int> mapToRoot(const TwPoint<int>& pt) const;

    TwPoint<int> mapFromParent(const TwPoint<int>& pt) const;
    TwPoint<int> mapFromRoot(const TwPoint<int>& pt) const;

    TwRect<int> mapToParent(const TwRect<int>& rc) const;
    TwRect<int> mapToRoot(const TwRect<int>& rc) const;

    TwRect<int> mapFromParent(const TwRect<int>& rc) const;
    TwRect<int> mapFromRoot(const TwRect<int>& rc) const;

    static TwPoint<int> mapPoint(const TwWidget* dst, const TwWidget* src, const TwPoint<int>& ptInSrc);
    static TwRect<int> mapRect(const TwWidget* dst, const TwWidget* src, const TwRect<int>& rcInSrc);

    TwPoint<int> mapToWindow(const TwPoint<int>& ptInLocal) const;
    TwPoint<int> mapFromWindow(const TwPoint<int>& ptInWindow) const;
    TwRect<int> mapToWindow(const TwRect<int>& rcInLocal) const;
    TwRect<int> mapFromWindow(const TwRect<int>& rcInWindow) const;

    void setBackgroundColor(const TwColor& color);
    TwColor backgroundColor() const { return m_backGroundColor; }

    virtual void paintWidgetAndChildren(TwPainter*);
    virtual void paintBackground(TwPainter*);
    virtual void paint(TwPainter*);
    virtual void paintChildren(TwPainter*);

public :
    TwSignal<void()> sigVisibleChanged;
    TwSignal<void()> sigEnableChanged;
    TwSignal<void()> sigBoundChanged;

protected:
    void setRootWidget(TwWidget* root);
    //widget tree, use for event handle
    //layout system do not use this
    //do not call this two method, instead call setParentWidget
    void addChildWidget(TwWidget* w);
    void removeChildWidget(TwWidget* w);

    void syncAllChildrenOrder();
    void sortChildren();

    void markDirtyChildrenOrder();
    void markDirtyLayout();
    void markDirtyRect();

    virtual void onVisibleChanged();
    virtual void onEnableChanged();
    virtual void onBoundChanged(const TwRect<int>& oldRect);

    virtual void onChildWidgetChange(TwWidget* w, TwWidget::ChangeType);

    virtual void onWidgetAdded(TwWidget* w);
    virtual void onWidgetRemoved(TwWidget* w);

    virtual void onEvent(TwEvent* ev);

    virtual void onResizeEvent(TwResizeEvent* ev);

    virtual void onMousePresseEvent(TwMouseEvent* ev);
    virtual void onMouseReleaseEvent(TwMouseEvent* ev);
    virtual void onMouseDBClicked(TwMouseEvent* ev);
    virtual void onMouseMoveEvent(TwMouseEvent* ev);

    virtual void onHoverMoveEvent(TwHoverEvent* ev);
    virtual void onHoverEnterEvent(TwHoverEvent* ev);
    virtual void onHoverLeaveEvent(TwHoverEvent* ev);
    virtual void onWheelEvent(TwWheelEvent* ev);

    virtual void onKeyPresseEvent(TwKeyEvent* ev);
    virtual void onKeyReleaseEvent(TwKeyEvent* ev);
    virtual void onCharEvent(TwCharEvent* ev);

    virtual void onFocusInEvent(TwFocusEvent* ev);
    virtual void onFocusOutEvent(TwFocusEvent* ev);

    virtual void onLayoutRequestEvent(TwEvent* ev);

protected:
    //widget tree
    TwWidget* m_parentWidget;
    TwWidget* m_rootWidget;
    std::vector<TwWidget*> m_layoutWidgets;
    std::vector<TwWidget*> m_childWidgets;

    Tw::SizePolicy m_widthPolicy;
    Tw::SizePolicy m_heightPolicy;

    TwRect<int> m_boundRect;
    int m_zOrder;

    //layout property
    Tw::Alignment m_hAlignment;
    Tw::Alignment m_vAlignment;
    int m_childWidgetSpacing;
    TwMargin<int> m_contentMargin;
    TwScopedPtr<TwLayoutManager> m_layout;

    //paint
    TwColor m_backGroundColor;

    //widget state
    bool m_visible;
    bool m_enabled;
    bool m_dirtyChildrenOrder;

    //event and flags
    int m_acceptButtons;
    int64 m_widgetFlags;
    TwScopedPtr<TwCursor> m_cursor;
};

TW_CLASS_REGISTER(TwWidget);
