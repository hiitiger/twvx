#pragma once

//////////////////////////////////////////////////////////////////////////

template<class T = int>
struct TwPoint
{
    typedef T ValueType;

    TwPoint()
        : m_x(), m_y()
    {
        ;
    }

    TwPoint(const TwPoint& other)
        : m_x(other.x()), m_y(other.y())
    {
        ;
    }

    TwPoint(ValueType xx, ValueType yy)
        : m_x(xx), m_y(yy)
    {
        ;
    }

    TwPoint& operator= (const TwPoint& other)
    {
        m_x = other.m_x;
        m_y = other.m_y;
        return *this;
    }

    void setX(ValueType xx)
    {
        m_x = xx;
    }

    ValueType x() const
    {
        return m_x;
    }

    void setY(ValueType yy)
    {
        m_y = yy;
    }

    ValueType y() const
    {
        return m_y;
    }

    bool operator == (TwPoint other) const
    {
        return twCompareValue(m_x, other.m_x)
            && twCompareValue(m_y, other.m_y);
    }

    bool operator != (TwPoint other) const
    {
        return !twCompareValue(m_x, other.m_x)
            || !twCompareValue(m_y, other.m_y);
    }

    TwPoint operator - () const
    {
        return TwPoint(-m_x, -m_y);
    }

    TwPoint operator + (const TwPoint& other) const
    {
        return TwPoint(m_x + other.x(), m_y + other.y());
    }

    TwPoint operator - (const TwPoint& other) const
    {
        return TwPoint(m_x - other.x(), m_y - other.y());
    }

    TwPoint& operator += (const TwPoint& pt)
    {
        m_x += pt.m_x;
        m_y += pt.m_y;
        return *this;
    }

    TwPoint& operator -= (const TwPoint& pt)
    {
        m_x -= pt.m_x;
        m_y -= pt.m_y;
        return *this;
    }

    TwPoint operator * (const double factor) const
    {
        return TwPoint((ValueType)(m_x * factor), (ValueType)(m_y * factor));
    }

    TwPoint operator / (const double divisor)  const
    {
        return TwPoint((ValueType)(m_x / divisor), (ValueType)(m_y / divisor));
    }

    TwPoint& operator *= (const double factor)
    {
        m_x = (ValueType)(m_x * factor);
        m_y = (ValueType)(m_y * factor);
        return *this;
    }

    TwPoint& operator /= (const double divisor)
    {
        m_x = (ValueType)(m_x / divisor);
        m_y = (ValueType)(m_x / divisor);
        return *this;
    }

private:
    ValueType m_x;
    ValueType m_y;
};

template<class T>
POINT twPointToWinPoint(const TwPoint<T>& pt)
{
    POINT point = { (int)pt.x(), (int)pt.y() };
    return point;
}

//////////////////////////////////////////////////////////////////////////

template<class T = int>
struct TwSize
{
    typedef T ValueType;

    TwSize()
        : m_width(), m_height()
    {
        ;
    }

    TwSize(const TwSize& other)
        : m_width(other.width()), m_height(other.height())
    {
        ;
    }

    TwSize(ValueType w, ValueType h)
        : m_width(w), m_height(h)
    {
        ;
    }

    TwSize& operator = (const TwSize& other)
    {
        m_width = other.width();
        m_height = other.height();
        return *this;
    }

    void setWidth(ValueType w) { m_width = w; }

    ValueType width() const { return m_width; }

    void setHeight(ValueType h) { m_height = h; }

    ValueType height() const { return m_height; }

    TwSize resize(ValueType w, ValueType h)
    {
        m_width = w;
        m_height = h;
    }

    bool isZero() const
    {
        return twIsZero(m_width)
            && twIsZero(m_height);
    }

    bool isValid() const
    {
        return m_width >= ValueType()
            && m_height >= ValueType();
    }

    bool operator == (TwSize other) const
    {
        return twCompareValue(m_width, other.width())
            && twCompareValue(m_height, other.height());
    }
    bool operator != (TwSize other) const
    {
        return !twCompareValue(m_width, other.width())
            || !twCompareValue(m_height, other.height());
    }

    bool operator < (TwSize other) const
    {
        return m_width < other.width()
            || m_height < other.height();
    }

    bool operator > (TwSize other) const
    {
        return m_width > other.width()
            && m_height > other.height();
    }

private:
    ValueType m_width;
    ValueType m_height;
};

template<class T>
SIZE twSizeToWinSize(const TwSize<T>& sz)
{
    SIZE size = { (int)sz.width(), (int)sz.height() };
    return size;
}

//////////////////////////////////////////////////////////////////////
//TwMargin
template<class T = int>
struct TwMargin
{
    typedef T ValueType;

    TwMargin() 
        : m_left(), m_top(), m_right(), m_bottom()
    {

    }

    TwMargin(ValueType left, ValueType top, ValueType right, ValueType bottom)
        : m_left(left), m_top(top), m_right(right), m_bottom(bottom)
    {

    }

    ValueType left() const
    {
        return m_left;
    }

    ValueType top() const
    {
        return m_top;
    }

    ValueType right() const
    {
        return m_right;
    }

    ValueType bottom() const
    {
        return m_bottom;
    }

private:
    ValueType m_left;
    ValueType m_top;
    ValueType m_right;
    ValueType m_bottom;
};

//TwMargin
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template<class T = int>
struct TwRect
{
    typedef T ValueType;

    TwRect()
        : m_x1(), m_y1(), m_x2(), m_y2()
    {
        ;
    }

    TwRect(const TwPoint<ValueType>& pt, const TwSize<ValueType> sz)
        :m_x1(pt.x()), m_y1(pt.y()), m_x2(pt.x() + sz.width()), m_y2(pt.y() + sz.height())
    {
        ;
    }

    TwRect(const TwPoint<ValueType>& ptTopLeft, const TwPoint<ValueType>& ptBottomRight)
        :m_x1(ptTopLeft.x()), m_y1(ptTopLeft.y()), m_x2(ptBottomRight.x()), m_y2(ptBottomRight.y())
    {
        ;
    }

    TwRect(ValueType left, ValueType top, ValueType right, ValueType bottom)
        : m_x1(left), m_y1(top), m_x2(right), m_y2(bottom)
    {
        ;
    }

    TwRect& operator = (const TwRect& other)
    {
        m_x1 = other.m_x1;
        m_y1 = other.m_y1;
        m_x2 = other.m_x2;
        m_y2 = other.m_y2;
        return *this;
    }

    bool isValid() const
    {
        return m_x1 < m_x2 && m_y1 < m_y2;
    }

    TwPoint<ValueType> topLeft() const
    {
        return TwPoint<ValueType>(m_x1, m_y1);
    }

    TwPoint<ValueType> bottomRight() const
    {
        return TwPoint<ValueType>(m_x2, m_y2);
    }

    ValueType left() const
    {
        return m_x1;
    }

    ValueType right() const
    {
        return m_x2;
    }

    ValueType top() const
    {
        return m_y1;
    }

    ValueType bottom() const
    {
        return m_y2;
    }

    ValueType width() const
    {
        return m_x2 - m_x1;
    }

    ValueType height() const
    {
        return m_y2 - m_y1;
    }

    TwSize<ValueType> size() const
    {
        return TwSize<ValueType>(m_x2 - m_x1, m_y2 - m_y1);
    }

    TwRect& resize(const TwSize<ValueType>& sz)
    {
        setWidth(sz.width());
        setHeight(sz.height());
        return *this;
    }

    TwRect& resize(ValueType w, ValueType h)
    {
        setWidth(w);
        setHeight(h);
        return *this;
    }

    TwRect& enlarge(ValueType w, ValueType h)
    {
        m_x2 += w;
        m_y2 += h;
        return *this;
    }

    TwRect& setWidth(ValueType w)
    {
        m_x2 = m_x1 + w;
        return *this;
    }

    TwRect& enlargeWidth(ValueType w)
    {
        m_x2 += w;
        return *this;
    }

    TwRect& setHeight(ValueType h)
    {
        m_y2 = m_y1 + h;
        return *this;
    }

    TwRect& enlargeHeight(ValueType h)
    {
        m_y2 += h;
        return *this;
    }

    TwRect& moveTo(const TwPoint<ValueType>& pt)
    {
        ValueType w = width();
        ValueType h = height();
        m_x1 = pt.x();
        m_y1 = pt.y();
        m_x2 = m_x1 + w;
        m_y2 = m_y1 + h;
        return *this;
    }

    TwRect movedTo(const TwPoint<ValueType>& pt) const
    {
        TwRect rc = *this;

        return rc.moveTo(pt);
    }

    TwRect& moveTo(ValueType x, ValueType y)
    {
        ValueType w = width();
        ValueType h = height();
        m_x1 = x;
        m_y1 = y;
        m_x2 = m_x1 + w;
        m_y2 = m_y1 + h;
        return *this;
    }

    TwRect movedTo(ValueType x, ValueType y) const
    {
        TwRect rc = *this;

        return rc.moveTo(x, y);
    }

    TwRect& adjust(ValueType dx)
    {
        m_x1 += dx;
        m_y1 += dx;
        m_x2 -= dx;
        m_y2 -= dx;
        return *this;
    }

    TwRect adjusted(ValueType dx) const
    {
        TwRect rc = *this;
        rc.m_x1 += dx;
        rc.m_y1 += dx;
        rc.m_x2 -= dx;
        rc.m_y2 -= dx;
        return rc;
    }

    TwRect& adjust(const TwMargin<ValueType>& margin)
    {
        m_x1 += margin.left();
        m_y1 += margin.top();
        m_x2 -= margin.right();
        m_y2 -= margin.bottom();
        return *this;
    }

    TwRect adjusted(const TwMargin<ValueType>& margin)
    {
        TwRect rc = *this;
        rc.m_x1 += margin.left();
        rc.m_y1 += margin.top();
        rc.m_x2 -= margin.right();
        rc.m_y2 -= margin.bottom();
        return rc;
    }

    TwRect& setLeft(ValueType left)
    {
        m_x2 = left + width();
        m_x1 = left;
        return *this;
    }

    TwRect& setTop(ValueType top)
    {
        m_y2 = top + height();
        m_y1 = top;
        return *this;
    }

    bool contains(ValueType x, ValueType y) const
    {
        return  x >= m_x1
            &&  y >= m_y1
            &&  x <= m_x2
            &&  y <= m_y2;
    }

    bool contains(const TwPoint<ValueType>& pt) const
    {
        return contains(pt.x(), pt.y());
    }

    bool isIntersectWith(const TwRect& rect) const
    {
        TwRect rc = intersectedWith(rect);
        return rc.isValid();
    }

    TwRect& intersectWith(const TwRect& rect)
    {
        m_x1 = twMax(m_x1, rect.m_x1);
        m_x2 = twMin(m_x2, rect.m_x2);
        m_y1 = twMax(m_y1, rect.m_y1);
        m_y2 = twMin(m_y2, rect.m_y2);
        return *this;
    }

    TwRect intersectedWith(const TwRect& rect) const
    {
        ValueType ix1 = twMax(m_x1, rect.m_x1);
        ValueType ix2 = twMin(m_x2, rect.m_x2);
        ValueType iy1 = twMax(m_y1, rect.m_y1);
        ValueType iy2 = twMin(m_y2, rect.m_y2);

        return TwRect(ix1, iy1, ix2, iy2);
    }

    bool operator == (const TwRect& other) const
    {
        return twCompareValue(m_x1, other.m_x1)
            && twCompareValue(m_y1, other.m_y1)
            && twCompareValue(m_x2, other.m_x2)
            && twCompareValue(m_y2, other.m_y2);
    }

    bool operator != (const TwRect& other) const
    {
        return !twCompareValue(m_x1, other.m_x1)
            || !twCompareValue(m_y1, other.m_y1)
            || !twCompareValue(m_x2, other.m_x2)
            || !twCompareValue(m_y2, other.m_y2);
    }

private:
    ValueType m_x1;
    ValueType m_y1;
    ValueType m_x2;
    ValueType m_y2;
};

template<class T>
RECT twRectToWinRect(const TwRect<T>& rc)
{
    RECT rect = { (int)rc.left(), (int)rc.top(), (int)rc.right(), (int)rc.bottom() };
    return rect;
}

