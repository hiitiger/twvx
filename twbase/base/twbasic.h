#pragma once
#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <stack>
#include <queue>
#include <list>
#include <memory>
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <numeric>
#include <chrono>
#include <atomic>
#include <thread>
#include <mutex>


//////////////////////////////////////////////////////////////////////////
#include "twtype.h"
#include "twassert.h"

#ifdef _DEBUG
#define  DAssert(e) twAssert((bool)(!!(e)), __FILE__, __FUNCTION__, __LINE__,"")
#define  DAssert_M(e, msg) twAssert((bool)(!!(e)), __FILE__, __FUNCTION__, __LINE__, msg)
#else
#define  DAssert(e) 
#define  DAssert_M(e, msg)  
#endif // _DEBUG

#define BREAK_IF(e)  if(!!(e)) {break;}
#define BREAK_IFNOT(e) if(!(e)) {break;}

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

inline double getMSecDiff(int64_t cur, int64_t last, int64_t countsPerSec)
{
    int64_t ticks = (cur - last);
    int64_t seconds = ticks / countsPerSec;
    int64_t uSeconds = ((ticks - seconds * countsPerSec) * 1000000 / countsPerSec);

    double mSeconds = (seconds * 1000000 + uSeconds) / 1000.0;
    return mSeconds;
}

class ConsolePerfTimer
{
    int64_t start_ = 0;
    const char* msg_;
public:
    ConsolePerfTimer(const char* msg)
        : msg_(msg)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&start_);
    }

    ~ConsolePerfTimer()
    {
        int64_t end_;
        QueryPerformanceCounter((LARGE_INTEGER*)&end_);
        int64_t _countPerSecs;
        QueryPerformanceFrequency((LARGE_INTEGER*)&_countPerSecs);

        double timeCost = getMSecDiff(end_, start_, _countPerSecs);
        printf("%s cost %f MSECS\n", msg_, timeCost);
    }
};
#define  PRINT_TIME(msg) ConsolePerfTimer print_time_##msg(#msg)
