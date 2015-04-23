#include "stable.h"
#include "twvar.h"


//////////////////////////////////////////////////////////////////////////
#define getNumericValue(typename)\
    DAssert(type_ == v_##typename);\
    switch (type_)\
    {\
        case TwVar::v_null:\
        return typename(0);\
        case TwVar::v_char:\
        return typename(var_.charValue);\
        case TwVar::v_uchar:\
        return typename(var_.ucharValue);\
        case TwVar::v_bool:\
        return typename(var_.boolValue ? 1:0);\
        case TwVar::v_int:\
        return typename(var_.intValue);\
        case TwVar::v_uint:\
        return typename(var_.uintValue);\
        case TwVar::v_int64:\
        return typename(var_.int64Value);\
        case TwVar::v_uint64:\
        return typename(var_.uint64Value);\
        case TwVar::v_float:\
        return typename(var_.floatValue);\
        case TwVar::v_double:\
        return typename(var_.doubleValue);\
        case TwVar::v_str:\
        return typename(0);\
        case TwVar::v_ptr:\
        return typename(0);\
        case TwVar::v_var:\
        return typename(0);\
        default:\
        break;\
    }\
     return typename(0);


TwVar::TwVar()
    :type_(v_null)
{
    var_.uint64Value = 0;
}

TwVar::~TwVar()
{
    clear();
}

TwVar::TwVar( const TwVar& other )
{
    type_ = v_null;
    (*this) = other;
}

TwVar::TwVar( char v )
{
    type_ = v_char;
    var_.charValue = v;
}

TwVar::TwVar( unsigned char v )
{
    type_ = v_uchar;
    var_.ucharValue = v;
}

TwVar::TwVar( bool v )
{
    type_ = v_bool;
    var_.boolValue = v;
}

TwVar::TwVar( int v )
{
    type_ = v_int;
    var_.intValue = v;
}

TwVar::TwVar( unsigned int v )
{
    type_ = v_uint;
    var_.uintValue = v;
}

TwVar::TwVar( int64 v )
{
    type_ = v_int64;
    var_.int64Value = v;
}

TwVar::TwVar( uint64 v )
{
    type_ = v_uint64;
    var_.uint64Value = v;
}

TwVar::TwVar( float v )
{
    type_ = v_float;
    var_.floatValue = v;
}

TwVar::TwVar( double v )
{
    type_ = v_double;
    var_.doubleValue = v;
}

TwVar::TwVar(const wchar_t* v)
{
    type_ = v_str;
    var_.strValue = new std::wstring(v);
}

TwVar::TwVar(const std::wstring& v)
{
    type_ = v_str;
    var_.strValue = new std::wstring(v);
}

TwVar& TwVar::operator=(const TwVar& other)
{
    if (this == &other)
    {
        return *this;
    }
    clear();

    switch (other.type_)
    {
    case v_null:
        break;
    case v_char:
        var_.charValue = other.var_.charValue;
        break;
    case v_uchar:
        var_.ucharValue = other.var_.ucharValue;
        break;
    case v_bool:
        var_.boolValue = other.var_.boolValue;
        break;
    case v_int:
        var_.intValue = other.var_.intValue;
        break;
    case v_uint:
        var_.uintValue = other.var_.uintValue;
        break;
    case v_int64:
        var_.int64Value = other.var_.int64Value;
        break;
    case v_uint64:
        var_.uint64Value = other.var_.uint64Value;
        break;
    case v_float:
        var_.floatValue = other.var_.floatValue;
        break;
    case v_double:
        var_.doubleValue = other.var_.doubleValue;
        break;
    case v_str:
        var_.strValue = new std::wstring(*(other.var_.strValue));
        break;
    case v_ptr:
        var_.ptrValue = other.var_.ptrValue->clone();
        break;
    case v_var:
        var_.varValue = other.var_.varValue->clone();
        break;
    default:
        break;
    }

    type_ = other.type_;
    return *this;
}


void TwVar::clear()
{
    switch (type_)
    {
    case v_null:
        break;
    case v_char:
        break;
    case v_uchar:
        break;
    case v_bool:
        break;
    case v_int:
        break;
    case v_uint:
        break;
    case v_int64:
        break;
    case v_uint64:
        break;
    case v_float:
        break;
    case v_double:
        break;
    case v_str:
        delete var_.strValue;
        var_.strValue = nullptr;
        break;
    case v_ptr:
        delete var_.ptrValue;
        var_.ptrValue = nullptr;
        break;
    case v_var:
        delete var_.varValue;
        var_.varValue = nullptr;
        break;
    default:
        break;
    }
    type_ = v_null;
    var_.uint64Value = 0;
}

char TwVar::getChar() const
{
    getNumericValue(char);
}

uchar TwVar::getuchar() const
{
    getNumericValue(uchar);
}

int TwVar::getint() const
{
    getNumericValue(int);
}

uint TwVar::getuint() const
{
    getNumericValue(uint);
}

int64 TwVar::getint64() const
{
    getNumericValue(int64);
}

uint64 TwVar::getuint64() const
{
    getNumericValue(uint64);
}

float TwVar::getfloat() const
{
    getNumericValue(float);
} 

double TwVar::getdouble() const
{
    getNumericValue(double);
}

bool TwVar::getbool() const
{
    DAssert(type_ == v_bool);
    switch (type_)
    {
    case v_null:
        return false;
    case v_char:
        return !!var_.charValue;
    case v_uchar:
        return !!var_.ucharValue;
    case v_bool:
        return var_.boolValue;
    case v_int:
        return !!var_.intValue;
    case v_uint:
        return !!var_.uintValue;
    case v_int64:
        return !!var_.int64Value;
    case v_uint64:
        return !!var_.uint64Value;
    case v_float:
        return !!var_.floatValue;
    case v_double:
        return !!var_.doubleValue;
    case v_ptr:
        return var_.ptrValue->ptr() != 0;
    case v_var:
        return false;
    default:
        break;
    }
    return false;
}