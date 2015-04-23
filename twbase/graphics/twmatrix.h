#pragma once

/*
    row major matrix
*/

struct Tw_Matrix_3x2F
{
    float _11;
    float _12;
    float _21;
    float _22;
    float _31;
    float _32;
};

struct Tw_Matrix_3x3F
{
    float _11;
    float _12;
    float _13;
    float _21;
    float _22;
    float _23;
    float _31;
    float _32;
    float _33;
};

   

/*
                |m11 m12 m13|         x*m11+y*m21+z*m31
    [x, y, z] * |m21 m22 m23|  =    [ x*m12+y*m22+z*m32 ]
                |m31 m32 m33|         x*m31+y*m23+z*m33   
   
    translate : (x, y) -->(x+dy, x+dy)
                |1. 0. 0.|       
    [x, y, 1] * |0. 1. 0.|     = [x + dx, y +dy, 1.0]
                |dx dy 1.|       


    scale : (x, y) -->(x * sx, y * sy)
                |sx 0. 0.|       
    [x, y, 1] * |0. sy 0.|     = [x*sx, y*sy,  1.0 ]
                |0. 0. 1.|       
  
    shear : (x, y) -->(x + y*sx, y+ x * sy)
                |1. 0. 0.|        
    [x, y, 1] * |sx 1. 0.|     = [ x+y*sx, y, 1.0 ]
                |0. 0. 1.|         

                |1. sy  0.|           
    [x, y, 1] * |0. 1. 0.|     = [ x, y+x*sy, 1.0]
                |0. 0. 1.|          

                |1. 0. 0.|    |1. sy 0.|       |1. sy 0.|       
    [x, y, 1] * |sx 1. 0.| *  |0. 1. 0.|     = |sx 1. 0.|    = [x+y*sx, y+x*sy, 1.0]
                |0. 0. 1.|    |0. 0. 1.|       |0. 0. 1.|        

    rotate : (1, 0) -->(cos(R), sin(R)), (0, 1) -->(-sin(R), cos(R))
                |cos(R)  sin(R)  0.|        
    [x, y, 1] * |-sin(R) cos(R)  0.|    = [x*cos(R)-y*sin(R), x*sin(R)+y*cos(R) ,  1.0 ]
                |0.      0.      1.|               

*/

enum  TwMatrixOrder
{
    MatrixOrder_Prepend,
    MatrixOrder_append,
};

class TW_BASE_API TwMatrix : public Tw_Matrix_3x2F
{
public:

    TwMatrix()
    {
        setIdentity();
    }
    TwMatrix(float _11, float _12, float _21, float _22, float dx, float dy)
    {
        this->_11 = _11;
        this->_12 = _12;
        this->_21 = _21;
        this->_22 = _22;
        this->_31 = dx;
        this->_32 = dy;
    }

    void setIdentity()
    {
        _11 = 1.0;
        _12 = 0.0;
        _21 = 0.0;
        _22 = 1.0;
        _31 = 0.0;
        _32 = 0.0;
    }

    float determinant() const
    {
        return _11 * _22 - _12 * _21;
    }

    TwMatrix inverted() const
    {
        float det = determinant();
        if (det != 0.f)
        {
            det = 1.0f / det;
            TwMatrix matrix(_22 * det, _12 * -det, -_21 * det, _11 * det, \
            _21 * _32 - _22 * _31, _11 * _32 - _12 * _31);
            return matrix;
        }
        else
        {
            return *this;
        }
    }

    static TwMatrix identity()
    {
        TwMatrix matrix(1.0, 0.0, 0.0, 1.0, 0.0, 0.0 );
        return matrix;
    }

    static TwMatrix translation(float dx, float dy)
    {
        TwMatrix matrix(1.0, 0.0, 0.0, 1.0, dx, dy);
        return matrix;
    }

    static TwMatrix scale(float factorX, float factorY)
    {
        TwMatrix matrix(factorX, 0.0, 0.0, factorY, 0.0, 0.0);
        return matrix;
    }

    static TwMatrix rotation(float degree)
    {
        float radias = degree * TW_PI_F /180.0f;
        TwMatrix matrix(::cos(radias), sin(radias), -(::sin(radias)), ::cos(radias), 0.0, 0.0);
        return matrix;
    }

    static TwMatrix rotation(float degree, float dx, float dy)
    {
        float radias = degree * TW_PI_F /180.0f;
        const float cosRad = ::cos (radias);
        const float sinRad = ::sin (radias);

        TwMatrix matrix(cosRad, sinRad, -(sinRad), cosRad, \
            -dx*cosRad + dy*sinRad + dx, -dx*sinRad - dy*cosRad + dy);
        return matrix;
    }

    static TwMatrix shear(float sx, float sy)
    {
        TwMatrix matrix(1.0, sy, sx, 1.0, 0.0, 0.0);
        return matrix;
    }

    TwMatrix& operator*=(const TwMatrix& m)
    {
        float r11 = _11 * m._11 + _12 * m._21;
        float r12 = _11 * m._12 + _12 * m._22;
        float r21 = _21 * m._11 + _22 * m._21;
        float r22 = _21 * m._12 + _22 * m._22;
        float r31 = _31 * m._11 + _32 * m._21 + m._31;
        float r32 = _31 * m._12 + _32 * m._22 + m._32;
        _11 = r11;
        _12 = r12;
        _21 = r21;
        _22 = r22;
        _31 = r31;
        _32 = r32;
        return *this;
    }

    TwMatrix operator*(const TwMatrix& m) const
    {
        float r11 = _11 * m._11 + _12 * m._21;
        float r12 = _11 * m._12 + _12 * m._22;
        float r21 = _21 * m._11 + _22 * m._21;
        float r22 = _21 * m._12 + _22 * m._22;
        float r31 = _31 * m._11 + _32 * m._21 + m._31;
        float r32 = _31 * m._12 + _32 * m._22 + m._32;
        return TwMatrix(r11, r12, r21, r22, r31, r32);
    }


    bool operator == (const TwMatrix& m) const
    {
        return _11 == m._11
            && _12 == m._12
            && _21 == m._21
            && _22 == m._22
            && _31 == m._31
            && _32 == m._32;
    }

    bool operator != (const TwMatrix& m) const
    {
        return _11 != m._11
            || _12 != m._12
            || _21 != m._21
            || _22 != m._22
            || _31 != m._31
            || _32 != m._32;
    }
};