#pragma once



inline __int64 getMicroSecDiff(__int64 cur, __int64 last, __int64 countsPerSec)
{
    __int64 ticks = (cur - last);
    __int64 seconds = ticks / countsPerSec;
    __int64 uSeconds = ((ticks - seconds * countsPerSec) * 1000000 / countsPerSec);

    __int64 microSeconds = (seconds * 1000000 + uSeconds);
    return microSeconds;
}

inline double getMilliSecDiff(__int64 cur, __int64 last, __int64 countsPerSec)
{
    __int64 microSeconds = getMicroSecDiff(cur, last, countsPerSec);
    double milliSeconds = (double)microSeconds / 1000.0;
    return milliSeconds;
}


class TW_BASE_API TwPerfCounter
{
    int64_t tickPerSec_ = 0;
    double tickPerMicroSec_ = 0;

public:
    TwPerfCounter()
    {
        QueryPerformanceFrequency((LARGE_INTEGER*)&tickPerSec_);
        tickPerMicroSec_ = tickPerSec_ / 1000000.0;
    }

    int64_t systemTickFreqency() const
    {
        return tickPerSec_;
    }

    int64_t systemTick() const
    {
        int64_t now;
        QueryPerformanceCounter((LARGE_INTEGER*)&now);
        return now;
    }

    double tickPerMicroSec() const
    {
        return tickPerMicroSec_;
    }

    int64_t microSecsNow() const
    {
        int64_t now;
        QueryPerformanceCounter((LARGE_INTEGER*)&now);
        return static_cast<int64_t>(now / tickPerMicroSec_);
    }

    int64_t milliSecsNow() const
    {
        int64_t now;
        QueryPerformanceCounter((LARGE_INTEGER*)&now);
        return static_cast<int64_t>(now / (1000.0*tickPerMicroSec_));
    }

};

class TW_BASE_API TwTimeTick
{
    int64_t microSecs_ = 0;
public:
    TwTimeTick() = default;

    static TwTimeTick highResNow()
    {
        using namespace std::chrono;
        auto timePoint = steady_clock::now();
        auto cur = timePoint.time_since_epoch();
        int64_t microSecs = duration_cast<microseconds>(cur).count();

        TwTimeTick timeTick(microSecs);
        return timeTick;
    }

    static TwTimeTick now()
    {
        return highResNow();
    }

    bool isNull() const
    {
        return microSecs_ == 0;
    }

    void setZero()
    {
        microSecs_ = 0;
    }

    int64_t microSecs() const
    {
        return microSecs_;
    }

    int64_t milliSecs() const
    {
        return microSecs_ / 1000;
    }

    TwTimeTick& addMicroSecs(int64_t microSeconds)
    {
        microSecs_ += microSeconds;
        return *this;
    }

    TwTimeTick& minusMicroSecs(int64_t microSeconds)
    {
        microSecs_ -= microSeconds;
        return *this;
    }

    TwTimeTick& addMilliSecs(int64_t milliSconds)
    {
        microSecs_ += (milliSconds * 1000);
        return *this;
    }

    TwTimeTick& minusMilliSecs(int64_t milliSconds)
    {
        microSecs_ -= (milliSconds * 1000);
        return *this;
    }

    TwTimeTick& operator += (const TwTimeTick& t)
    {
        microSecs_ += t.microSecs_;
        return *this;
    }

    TwTimeTick& operator -= (const TwTimeTick& t)
    {
        microSecs_ -= t.microSecs_;
        return *this;
    }

    TwTimeTick operator - (const TwTimeTick& t)
    {
        TwTimeTick result = *this;
        result -= t;
        return result;
    }

    bool operator==(const TwTimeTick& t)const { return microSecs_ == t.microSecs_; }
    bool operator!=(const TwTimeTick& t)const { return microSecs_ != t.microSecs_; }
    bool operator <(const TwTimeTick& t)const { return microSecs_ < t.microSecs_; }
    bool operator<=(const TwTimeTick& t)const { return microSecs_ <= t.microSecs_; }
    bool operator >(const TwTimeTick& t)const { return microSecs_ > t.microSecs_; }
    bool operator>=(const TwTimeTick& t)const { return microSecs_ >= t.microSecs_; }

private:
    TwTimeTick(int64_t t)
        : microSecs_(t)
    {}
};

enum class TW_BASE_API TwTimeSpec
{
    UTC,
    Local
};

class TW_BASE_API TwDateTime
{
    int year_ = 0;
    int month_ = 0;
    int dayOfWeek_ = 0;
    int dayOfMonth_ = 0;
    int dayOfYear_ = 0;
    int hour_ = 0;
    int minute_ = 0;
    int sec_ = 0;
    int milliSec_ = 0;
    int64_t milliSecsUTC_ = 0;
    TwTimeSpec timeSpec_ = TwTimeSpec::Local;
public:
    TwDateTime() = default;

    static TwDateTime now();
    static TwDateTime nowUTC();
    static int64_t currentMilliSecsSinceEpoch();

    static TwDateTime fromUTCTimeStamp(std::uint32_t sec);
    static TwDateTime fromTimeStamp(std::uint32_t sec);

    void addMilliSecs(int64_t msecs);

    TwTimeSpec timeSpec() const { return timeSpec_; }
    int year() const { return year_; }
    int month() const { return month_; }
    int dayOfWeek() const { return dayOfWeek_; }
    int dayOfMonth() const { return dayOfMonth_; }
    int dayOfYear() const { return dayOfYear_; }
    int hour() const { return hour_; }
    int minute() const { return minute_; }
    int sec() const { return sec_; }
    int milliSec() const { return milliSec_; }

};

inline TwDateTime TwDateTime::now()
{
    using namespace std::chrono;
    auto timePoint = system_clock::now();
    auto cur = timePoint.time_since_epoch();
    milliseconds ms = duration_cast<milliseconds>(cur);
    int milliSecs = static_cast<int>(ms.count() % 1000);
    time_t curtime = system_clock::to_time_t(timePoint);

    //std::tm* curtm = localtime(&curtime);
    std::tm buf;
    localtime_s(&buf, &curtime);
    std::tm* curtm = &buf;

    TwDateTime dateTime;
    dateTime.year_ = curtm->tm_year + 1900;
    dateTime.month_ = curtm->tm_mon + 1;
    dateTime.dayOfWeek_ = curtm->tm_wday + 1;
    dateTime.dayOfMonth_ = curtm->tm_mday;
    dateTime.dayOfYear_ = curtm->tm_yday;
    dateTime.hour_ = curtm->tm_hour;
    dateTime.minute_ = curtm->tm_min;
    dateTime.sec_ = curtm->tm_sec;
    dateTime.milliSec_ = milliSecs;
    dateTime.milliSecsUTC_ = ms.count();
    dateTime.timeSpec_ = TwTimeSpec::Local;
    return dateTime;
}

inline TwDateTime TwDateTime::nowUTC()
{
    using namespace std::chrono;
    auto timePoint = system_clock::now();
    auto cur = timePoint.time_since_epoch();
    milliseconds ms = duration_cast<milliseconds>(cur);
    int milliSecs = static_cast<int>(ms.count() % 1000);
    time_t curtime = system_clock::to_time_t(timePoint);
    //std::tm* curtm = gmtime(&curtime);
    std::tm buf;
    gmtime_s(&buf, &curtime);
    std::tm* curtm = &buf;

    TwDateTime dateTime;
    dateTime.year_ = curtm->tm_year + 1900;
    dateTime.month_ = curtm->tm_mon + 1;
    dateTime.dayOfWeek_ = curtm->tm_wday + 1;
    dateTime.dayOfMonth_ = curtm->tm_mday;
    dateTime.dayOfYear_ = curtm->tm_yday;
    dateTime.hour_ = curtm->tm_hour;
    dateTime.minute_ = curtm->tm_min;
    dateTime.sec_ = curtm->tm_sec;
    dateTime.milliSec_ = milliSecs;
    dateTime.milliSecsUTC_ = ms.count();
    dateTime.timeSpec_ = TwTimeSpec::UTC;
    return dateTime;
}

inline int64_t TwDateTime::currentMilliSecsSinceEpoch()
{
    using namespace std::chrono;
    milliseconds ms = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
        );
    return ms.count();
}

inline TwDateTime TwDateTime::fromUTCTimeStamp(std::uint32_t sec)
{
    TwDateTime dateTime;
    time_t curtime = sec;
    std::tm buf;
    gmtime_s(&buf, &curtime);
    std::tm* curtm = &buf;

    dateTime.year_ = curtm->tm_year + 1900;
    dateTime.month_ = curtm->tm_mon + 1;
    dateTime.dayOfWeek_ = curtm->tm_wday + 1;
    dateTime.dayOfMonth_ = curtm->tm_mday;
    dateTime.dayOfYear_ = curtm->tm_yday;
    dateTime.hour_ = curtm->tm_hour;
    dateTime.minute_ = curtm->tm_min;
    dateTime.sec_ = curtm->tm_sec;
    dateTime.milliSec_ = 0;
    dateTime.milliSecsUTC_ = (int64_t)sec * 1000;
    dateTime.timeSpec_ = TwTimeSpec::UTC;
    return dateTime;
}

inline TwDateTime TwDateTime::fromTimeStamp(std::uint32_t sec)
{
    TwDateTime dateTime;
    time_t curtime = sec;
    std::tm buf;
    localtime_s(&buf, &curtime);
    std::tm* curtm = &buf;

    dateTime.year_ = curtm->tm_year + 1900;
    dateTime.month_ = curtm->tm_mon + 1;
    dateTime.dayOfWeek_ = curtm->tm_wday + 1;
    dateTime.dayOfMonth_ = curtm->tm_mday;
    dateTime.dayOfYear_ = curtm->tm_yday;
    dateTime.hour_ = curtm->tm_hour;
    dateTime.minute_ = curtm->tm_min;
    dateTime.sec_ = curtm->tm_sec;
    dateTime.milliSec_ = 0;
    dateTime.milliSecsUTC_ = (int64_t)sec * 1000;
    dateTime.timeSpec_ = TwTimeSpec::Local;
    return dateTime;
}

inline void TwDateTime::addMilliSecs(int64_t msecs)
{
    int64_t utcTimeMSecs = milliSecsUTC_ + msecs;
    int milliSecs = static_cast<int>(utcTimeMSecs % 1000);
    time_t curtime = utcTimeMSecs / 1000;

    std::tm buf;
    if (timeSpec_ == TwTimeSpec::Local)
    {
        localtime_s(&buf, &curtime);
    }
    else
    {
        gmtime_s(&buf, &curtime);
    }
    std::tm* curtm = &buf;

    year_ = curtm->tm_year + 1900;
    month_ = curtm->tm_mon + 1;
    dayOfWeek_ = curtm->tm_wday + 1;
    dayOfMonth_ = curtm->tm_mday;
    dayOfYear_ = curtm->tm_yday;
    hour_ = curtm->tm_hour;
    minute_ = curtm->tm_min;
    sec_ = curtm->tm_sec;
    milliSec_ = milliSecs;
    milliSecsUTC_ = utcTimeMSecs;
}

inline std::string toString(const TwDateTime& dateTime)
{
    std::stringstream ss;
    ss << dateTime.year()
        << "-" << dateTime.month()
        << "-" << dateTime.dayOfMonth()
        << " " << dateTime.hour()
        << ":" << dateTime.minute()
        << ":" << dateTime.sec()
        << ":" << dateTime.milliSec();

    return ss.str();
}
inline std::ostream& operator<<(std::ostream& os, const TwDateTime& dateTime)
{
    os << dateTime.year()
        << "-" << dateTime.month()
        << "-" << dateTime.dayOfMonth()
        << " " << dateTime.hour()
        << ":" << dateTime.minute()
        << ":" << dateTime.sec()
        << ":" << dateTime.milliSec()
        << std::endl;

    return os;
}



