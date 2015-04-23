#pragma once
#include <sstream>
#include "twtype.h"

static const int TTwLog_DebugConsole = 0;
static const int TTwLog_Debug = 1;
static const int TTwLog_Release = 2;
static const int TTwLog_Public = 3;

static const char TwXXXLog[] = "TwXXXLog";

typedef void (*_log_hook)(const std::string& data);

class TW_BASE_API TwLog
{
public:
    TwLog(const char* filter, int type, const char* file, const char* func, int line);
    ~TwLog();

    static void initLog(bool useDCLog = true, _log_hook = nullptr);
    static void uninitLog();

    TwLog& operator<<(bool d);
    TwLog& operator<<(char d);
    TwLog& operator<<(short d);
    TwLog& operator<<(unsigned short d);
    TwLog& operator<<(float d);
    TwLog& operator<<(double d);
    TwLog& operator<<(int d);
    TwLog& operator<<(unsigned int d);
    TwLog& operator<<(long d);
    TwLog& operator<<(unsigned long d);
    TwLog& operator<<(int64 d);
    TwLog& operator<<(uint64 d);
    TwLog& operator<<(const wchar_t* d);
    TwLog& operator<<(const std::wstring& d);
    TwLog& operator<<(const char* d);
    TwLog& operator<<(const std::string& d);
    TwLog& operator<<(void* d);

private:
    //TODO:
    std::stringstream m_logData;
    int m_type;
};


class TW_BASE_API TwEmptyLog
{
public:
    TwEmptyLog(const char* /*filter*/, int /*type*/, const char* /*file*/, const char* /*func*/, int /*line*/) {;}
    ~TwEmptyLog() {;}

    TwEmptyLog& operator<<(bool ) { return *this;}
    TwEmptyLog& operator<<(char ){ return *this;}
    TwEmptyLog& operator<<(short ){ return *this;}
    TwEmptyLog& operator<<(unsigned short ){ return *this;}
    TwEmptyLog& operator<<(float ){ return *this;}
    TwEmptyLog& operator<<(double ){ return *this;}
    TwEmptyLog& operator<<(int ){ return *this;}
    TwEmptyLog& operator<<(unsigned int ){ return *this; }
    TwEmptyLog& operator<<(long ){ return *this; }
    TwEmptyLog& operator<<(unsigned long ){ return *this; }
    TwEmptyLog& operator<<(int64 ) { return *this; }
    TwEmptyLog& operator<<(uint64 ) { return *this; }
    TwEmptyLog& operator<<(const wchar_t* ){ return *this;}
    TwEmptyLog& operator<<(const std::wstring& ){ return *this;}
    TwEmptyLog& operator<<(const char* ){ return *this;}
    TwEmptyLog& operator<<(const std::string& ){ return *this;}
    TwEmptyLog& operator<<(void* ){ return *this;}
};