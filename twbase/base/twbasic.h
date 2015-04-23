#pragma once
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <list>
#include <memory>
#include <algorithm>
#include <functional>



//////////////////////////////////////////////////////////////////////////
#include "twtype.h"
#include "twassert.h"
#include "twlog.h"

#ifdef _DEBUG
#define  DAssert(e) twAssert((bool)(!!(e)), __FILE__, __FUNCTION__, __LINE__,"")
#define  DAssert_M(e, msg) twAssert((bool)(!!(e)), __FILE__, __FUNCTION__, __LINE__, msg)
#else
#define  DAssert(e) 
#define  DAssert_M(e, msg)  
#endif // _DEBUG

#ifdef _DEBUG
#define  DLog(filter) TwLog(filter, TTwLog_Debug,  __FILE__, __FUNCTION__, __LINE__)
#define  DCLog(filter) TwLog(filter, TTwLog_DebugConsole,  __FILE__, __FUNCTION__, __LINE__)
#else
#define  DLog(filter) TwEmptyLog(filter, TTwLog_Debug,  __FILE__, __FUNCTION__, __LINE__)
#define  DCLog(filter) TwEmptyLog(filter, TTwLog_DebugConsole,  __FILE__, __FUNCTION__, __LINE__)
#endif // _DEBUG

#define  TempLog DCLog(TwXXXLog) 
#define  ReleaseLog(filter) TwLog(filter, TTwLog_Release,  __FILE__, __FUNCTION__, __LINE__)

#define  TraceLog 


//////////////////////////////////////////////////////////////////////////
inline int twRound(double d)
{
    return d >= 0.0 ? int(d + 0.5) : int(d - int(d-1) + 0.5) + int(d-1);
}

template <typename T>
inline T twAbs(const T &t) 
{ 
    return t >= 0 ? t : -t; 
}

template <typename T>
inline const T& twMin(const T& t1, const T& t2) 
{ 
    return t1 < t2 ? t1 : t2;
}

template <typename T>
inline const T& twMax(const T& t1, const T& t2) 
{ 
    return t1 > t2 ? t1 : t2;
}

inline bool twFuzzyCompare(float d1, float d2)
{
    return  twAbs(d1 - d2) <= 0.00001f * twMin(twAbs(d1),twAbs(d2));
}

inline bool twFuzzyCompare(double d1, double d2)
{
    return  twAbs(d1 - d2) <= 0.000000000001 * twMin(twAbs(d1),twAbs(d2));
}

inline bool twFuzzyIsZero(float f)
{
    return twAbs(f) <= 0.00001f;
}

inline bool twFuzzyIsZero(double d)
{
    return twAbs(d) <= 0.000000000001;
}

template<class T>
inline bool twIsZero(T a)
{
    return a == T();
}

template<class T>
inline bool twCompareValue(T a1, T a2)
{
    return a1 == a2;
}

template<>
inline bool twCompareValue(float a1, float a2)
{
    return twFuzzyCompare(a1, a2);
}

template<>
inline bool twCompareValue(double a1, double a2)
{
    return twFuzzyCompare(a1, a2);
}
//////////////////////////////////////////////////////////////////////////

static const double TW_PI_D = 3.14159265358979323846;  /* pi */
static const float TW_PI_F = 3.14159265f;  /* pi */

