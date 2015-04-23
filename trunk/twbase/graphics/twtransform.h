#pragma once
#include "twmatrix.h"

class TW_BASE_API TwAffineTransform
{
public:
    TwAffineTransform();
    TwAffineTransform(const TwMatrix& m);
    void setMatrix(const TwMatrix& m);

    TwAffineTransform& setIdentity();
    bool isIdentity() const;

    TwAffineTransform& translate(float dx, float dy, TwMatrixOrder order = MatrixOrder_Prepend);
    TwAffineTransform  translated(float dx, float dy, TwMatrixOrder order = MatrixOrder_Prepend) const;

    TwAffineTransform& scale(float factorX, float factorY, TwMatrixOrder order = MatrixOrder_Prepend);
    TwAffineTransform  scaled(float factorX, float factorY, TwMatrixOrder order = MatrixOrder_Prepend) const;

    TwAffineTransform& rotate(float degree, float x = 0.0f, float y = 0.0f, TwMatrixOrder order = MatrixOrder_Prepend);
    TwAffineTransform  rotated(float degree, float x = 0.0f, float y = 0.0f, TwMatrixOrder order = MatrixOrder_Prepend) const;

    TwAffineTransform& shear(float sx, float sy, TwMatrixOrder order = MatrixOrder_Prepend);
    TwAffineTransform  sheared(float sx, float sy, TwMatrixOrder order = MatrixOrder_Prepend) const;

    TwAffineTransform inverted() const;

    TwAffineTransform& concatTransform(const TwAffineTransform& other);
    TwAffineTransform& preConcatTransform(const TwAffineTransform& other);

    TwAffineTransform& operator*=(const TwAffineTransform &other);
    TwAffineTransform  operator*(const TwAffineTransform &other) const;

    bool operator==(const TwAffineTransform& other) const;
    bool operator!=(const TwAffineTransform& other) const;

    TwMatrix m_matrix;
};

inline bool TwAffineTransform::operator==(const TwAffineTransform& other) const
{
    return m_matrix == other.m_matrix;
}

inline bool TwAffineTransform::operator!=(const TwAffineTransform& other) const
{
    return m_matrix != other.m_matrix;
}