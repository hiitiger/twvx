#pragma once

//////////////////////////////////////////////////////////////////////////

template<class T>
struct Identity
{
    typedef T type;
};

template <typename T>
struct TwTypeInfo
{
    enum {
        isPointer = std::is_pointer<T>::value,
        isReference = std::is_reference<T>::value
    };
};

template<class T>
struct RemoveCVR
{
    typedef T type;
};

template<class T>
struct RemoveCVR< T& >
{
    typedef typename RemoveCVR<T>::type type;
};

template<class T>
struct RemoveCVR < const T >
{
    typedef typename RemoveCVR<T>::type type;
};

template<class T>
struct RemoveCVR < volatile T >
{
    typedef typename RemoveCVR<T>::type type;
};

//////////////////////////////////////////////////////////////////////////
#define  TW_PP_INT_N(X) TW_PP_INT_##X
#define  TW_PP_INT_INC(X) TW_PP_INT_INC_##X
#define  TW_PP_INT_DEC(X) TW_PP_INT_DEC_##X

#define  TW_PP_INT_0 0
#define  TW_PP_INT_1 1
#define  TW_PP_INT_2 2
#define  TW_PP_INT_3 3
#define  TW_PP_INT_4 4
#define  TW_PP_INT_5 5
#define  TW_PP_INT_6 6

#define  TW_PP_INT_INC_0 1
#define  TW_PP_INT_INC_1 2
#define  TW_PP_INT_INC_2 3
#define  TW_PP_INT_INC_3 4
#define  TW_PP_INT_INC_4 5
#define  TW_PP_INT_INC_5 6
#define  TW_PP_INT_INC_6 7

#define  TW_PP_INT_DEC_0 -1
#define  TW_PP_INT_DEC_1 0
#define  TW_PP_INT_DEC_2 1
#define  TW_PP_INT_DEC_3 2
#define  TW_PP_INT_DEC_4 3
#define  TW_PP_INT_DEC_5 4
#define  TW_PP_INT_DEC_6 5

#define  TW_PP_INT_GEN_N(X) TW_PP_INT_##X
#define  TW_PP_INT_GEN_0()

#define  TW_PP_INT_GEN(X) TW_PP_INT_GEN_##X

#define  TW_PP_COMMA ,
#define  TW_PP_CAT(A, B) A ## B