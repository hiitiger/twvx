#include "stable.h"
#include "twtransform.h"

TwAffineTransform::TwAffineTransform()
{

}

TwAffineTransform::TwAffineTransform( const TwMatrix& m )
    : m_matrix(m)
{

}

void TwAffineTransform::setMatrix( const TwMatrix& m )
{
    m_matrix = m;
}

TwAffineTransform& TwAffineTransform::setIdentity()
{
    m_matrix.setIdentity();
    return *this;
}

bool TwAffineTransform::isIdentity() const
{
    return  m_matrix._11 == 1.0
         && m_matrix._12 == 0.0
         && m_matrix._21 == 0.0
         && m_matrix._22 == 1.0
         && m_matrix._31 == 0.0
         && m_matrix._32 == 0.0;
}

TwAffineTransform& TwAffineTransform::translate(float dx, float dy, TwMatrixOrder order /*= MatrixOrder_Prepend*/)
{
    TwMatrix matrix = TwMatrix::translation(dx, dy);
    if (order == MatrixOrder_Prepend)
    {
        m_matrix = matrix*m_matrix;
    }
    else
    {
        m_matrix = m_matrix*matrix;
    }
    return *this;
}

TwAffineTransform TwAffineTransform::translated(float dx, float dy, TwMatrixOrder order /*= MatrixOrder_Prepend*/) const
{
    TwAffineTransform trans = *this;
    return trans.translate(dx, dy, order);
}

TwAffineTransform& TwAffineTransform::scale(float factorX, float factorY, TwMatrixOrder order /*= MatrixOrder_Prepend*/)
{
    TwMatrix matrix = TwMatrix::scale(factorX, factorY);
    if (order == MatrixOrder_Prepend)
    {
        m_matrix = matrix*m_matrix;
    }
    else
    {
        m_matrix = m_matrix*matrix;
    }
    return *this;
}

TwAffineTransform TwAffineTransform::scaled(float factorX, float factorY, TwMatrixOrder order /*= MatrixOrder_Prepend*/) const
{
    TwAffineTransform trans = *this;
    return trans.scale(factorX, factorY, order);
}

TwAffineTransform& TwAffineTransform::rotate(float degree, float x /*= 0.0f*/, float y /*= 0.0f*/, TwMatrixOrder order /*= MatrixOrder_Prepend*/)
{
    TwMatrix matrix = TwMatrix::rotation(degree, x, y);
    if (order == MatrixOrder_Prepend)
    {
        m_matrix = matrix*m_matrix;
    }
    else
    {
        m_matrix = m_matrix*matrix;
    }
    return *this;
}

TwAffineTransform TwAffineTransform::rotated(float degree, float x /*= 0.0f*/, float y /*= 0.0f*/, TwMatrixOrder order /*= MatrixOrder_Prepend*/) const
{
    TwAffineTransform trans = *this;
    return trans.rotate(degree, x, y, order);
}

TwAffineTransform& TwAffineTransform::shear(float sx, float sy, TwMatrixOrder order /*= MatrixOrder_Prepend*/)
{
    TwMatrix matrix = TwMatrix::shear(sx, sy);
    if (order == MatrixOrder_Prepend)
    {
        m_matrix = matrix*m_matrix;
    }
    else
    {
        m_matrix = m_matrix*matrix;
    }
    return *this;
}

TwAffineTransform TwAffineTransform::sheared(float sx, float sy, TwMatrixOrder order /*= MatrixOrder_Prepend*/) const
{
    TwAffineTransform trans = *this;
    return trans.shear(sx, sy, order);
}

TwAffineTransform TwAffineTransform::inverted() const
{
    TwAffineTransform trans(m_matrix.inverted());
    return trans;
}

TwAffineTransform& TwAffineTransform::concatTransform( const TwAffineTransform& other )
{
    m_matrix *= other.m_matrix;
    return *this;
}

TwAffineTransform& TwAffineTransform::preConcatTransform( const TwAffineTransform& other )
{
    m_matrix = other.m_matrix * m_matrix;
    return *this;
}

TwAffineTransform& TwAffineTransform::operator*=(const TwAffineTransform &other)
{
    m_matrix *= other.m_matrix;
    return *this;
}

TwAffineTransform TwAffineTransform::operator*(const TwAffineTransform &other) const
{
    TwAffineTransform trans = *this;
    return trans *= other;
}

