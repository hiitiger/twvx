#pragma once

template<class T>
class TwFunction;


template<>
class TwFunction<void()>
{
public:
    static const int ArgCount = 0;
};

template<class R>
class TwFunction<R()>
{
public:
    static const int ArgCount = 0;
};

template<class A1>
class TwFunction<void(A1)>
{
public:
    typedef A1 Arg1_Type;
    static const int ArgCount = 1;
};

template<class R, class A1>
class TwFunction<R(A1)>
{
public:
    typedef A1 Arg1_Type;
    static const int ArgCount = 1;
};

template<class A1, class A2>
class TwFunction<void(A1, A2)>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    static const int ArgCount = 2;
};

template<class R, class A1, class A2>
class TwFunction<R(A1, A2)>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    static const int ArgCount = 2;
};

template<class A1, class A2, class A3>
class TwFunction<void(A1, A2, A3)>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    static const int ArgCount = 3;
};

template<class R, class A1, class A2, class A3>
class TwFunction<R(A1, A2, A3)>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    static const int ArgCount = 3;
};

template<class A1, class A2, class A3, class A4>
class TwFunction<void(A1, A2, A3, A4)>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    typedef A4 Arg4_Type;
    static const int ArgCount = 4;
};

template<class R, class A1, class A2, class A3, class A4>
class TwFunction<R(A1, A2, A3, A4)>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    typedef A4 Arg4_Type;
    static const int ArgCount = 4;
};

template<class A1, class A2, class A3, class A4, class A5>
class TwFunction<void(A1, A2, A3, A4, A5)>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    typedef A4 Arg4_Type;
    typedef A5 Arg5_Type;
    static const int ArgCount = 5;
};

template<class R, class A1, class A2, class A3, class A4, class A5>
class TwFunction<R(A1, A2, A3, A4, A5)>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    typedef A4 Arg4_Type;
    typedef A5 Arg5_Type;
    static const int ArgCount = 5;
};

template<class A1, class A2, class A3, class A4, class A5, class A6>
class TwFunction<void(A1, A2, A3, A4, A5, A6)>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    typedef A4 Arg4_Type;
    typedef A5 Arg5_Type;
    typedef A6 Arg6_Type;
    static const int ArgCount = 6;
};

template<class R, class A1, class A2, class A3, class A4, class A5, class A6>
class TwFunction<R(A1, A2, A3, A4, A5, A6)>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    typedef A4 Arg4_Type;
    typedef A5 Arg5_Type;
    typedef A6 Arg6_Type;
    static const int ArgCount = 6;
};

template<class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
class TwFunction < R(A1, A2, A3, A4, A5, A6, A7) >
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    typedef A4 Arg4_Type;
    typedef A5 Arg5_Type;
    typedef A6 Arg6_Type;
    typedef A7 Arg7_Type;
    static const int ArgCount = 7;
};

template<class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class TwFunction < R(A1, A2, A3, A4, A5, A6, A7, A8) >
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    typedef A4 Arg4_Type;
    typedef A5 Arg5_Type;
    typedef A6 Arg6_Type;
    typedef A7 Arg7_Type;
    typedef A8 Arg8_Type;
    static const int ArgCount = 8;
};

#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_1(Signature) typename TwFunction<Signature>::Arg1_Type
#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_2(Signature) typename TwFunction<Signature>::Arg2_Type
#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_3(Signature) typename TwFunction<Signature>::Arg3_Type
#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_4(Signature) typename TwFunction<Signature>::Arg4_Type
#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_5(Signature) typename TwFunction<Signature>::Arg5_Type
#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_6(Signature) typename TwFunction<Signature>::Arg6_Type
#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_7(Signature) typename TwFunction<Signature>::Arg7_Type
#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_8(Signature) typename TwFunction<Signature>::Arg8_Type

#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_0(Signature, C) \

#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT(Signature, C) \
    TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_##C(Signature)

#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_0(Signature, X) 

#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_1(Signature, X) \
    TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_0(Signature, TW_PP_INT_DEC(X))\
    TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT(Signature, 1)

#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_2(Signature, X) \
    TW_PP_CAT(TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_1(Signature, TW_PP_INT_DEC(X)), TW_PP_COMMA) \
    TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT(Signature, 2)

#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_3(Signature, X) \
    TW_PP_CAT(TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_2(Signature, TW_PP_INT_DEC(X)), TW_PP_COMMA) \
    TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT(Signature, 3)

#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_4(Signature, X) \
    TW_PP_CAT(TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_3(Signature, TW_PP_INT_DEC(X)), TW_PP_COMMA) \
    TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT(Signature, 4)

#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_5(Signature, X) \
    TW_PP_CAT(TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_4(Signature, TW_PP_INT_DEC(X)), TW_PP_COMMA) \
    TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT(Signature, 5)

#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_6(Signature, X) \
    TW_PP_CAT(TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_5(Signature, TW_PP_INT_DEC(X)), TW_PP_COMMA) \
    TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT(Signature, 6)

#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_7(Signature, X) \
    TW_PP_CAT(TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_6(Signature, TW_PP_INT_DEC(X)), TW_PP_COMMA) \
    TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT(Signature, 7)

#define TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_8(Signature, X) \
    TW_PP_CAT(TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_7(Signature, TW_PP_INT_DEC(X)), TW_PP_COMMA) \
    TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT(Signature, 8)

#define  TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN(Signature, X) TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN_##X(Signature, X)





