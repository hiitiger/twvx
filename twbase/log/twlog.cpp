#include "stable.h"
#include <iostream>
#include "twlog.h"
#include "debugconsole.h"
#include "twbasic.h"

char* wcharStringToCharString(const wchar_t* content,int length)
{
    if(content == nullptr)
    {
        return nullptr;
    }

    if (length < 0 && length != -1)
    {
        return nullptr;
    }

    int c = WideCharToMultiByte(CP_ACP,0,content,length,0,0,0,0);
    if(c == 0)
    {
        return nullptr;
    }

    char *ao = new char[c+1];
    memset(ao,0,c+1);
    c = WideCharToMultiByte(CP_ACP,0,content,length,ao,c,0,0);
    if (c == 0)
    {
        delete [] ao;
        return "";
    }

    ao[c] = '\0';
    return ao;
}

std::string wstringTostring(const std::wstring& str)
{
    std::string retStr;

    char* res = wcharStringToCharString(str.c_str(), str.length());
    if (res)
    {
        retStr.assign(res, strlen(res)+1);
        delete [] res;
    }

    return retStr;
}


class LogFile
{
public:
    LogFile() {;}
    ~LogFile() {;}

    void log(const std::string& data, int type)
    {
        //TODO:
    }
    
    static LogFile* sLog;
};

LogFile* LogFile::sLog = nullptr;

_log_hook Log_hook = nullptr;
bool Log_useDCLog = false;

void doLog(const std::string& d, int type)
{
#ifdef _DEBUG
    if (Log_useDCLog)
    {
        if (type == TTwLog_DebugConsole)
        {
            std::cout<<d<<std::endl;
        }
    }
    else
#endif
    {
        LogFile::sLog->log(d, type);
    }
}

void TwLog::initLog( bool useDCLog /*= true*/, _log_hook hookfn/*= nullptr*/ )
{
    Log_useDCLog = useDCLog;
#ifdef _DEBUG
    if (Log_useDCLog)
    {
        DebugConsole::getConsole();
    }
#endif
    Log_hook = hookfn;
    LogFile::sLog = new LogFile;
}

void TwLog::uninitLog()
{
    delete LogFile::sLog;
    LogFile::sLog = nullptr;
}


TwLog::TwLog( const char* filter, int type, const char* file, const char* func, int line )
    : m_type(type)
{
    m_logData<<"filter: "<<filter<<" type: "<<type<<" file: "<<file<<" func: "<<func<<" line: "<<line<<" Log: ";
}

TwLog::~TwLog()
{
    doLog(m_logData.str(), m_type);
}

TwLog& TwLog::operator<<( bool d )
{
    m_logData<<( d ? "true" : "false");
    return *this;
}

TwLog& TwLog::operator<<( char d )
{
    m_logData<<d;
    return *this;
}

TwLog& TwLog::operator<<( short d )
{
    m_logData<<d;
    return *this;
}

TwLog& TwLog::operator<<( unsigned short d )
{
    m_logData<<d;
    return *this;
}

TwLog& TwLog::operator<<( float d )
{
    m_logData<<d;
    return *this;
}

TwLog& TwLog::operator<<( double d )
{
    m_logData<<d;
    return *this;
}

TwLog& TwLog::operator<<( int d)
{
    m_logData<<d;
    return *this;
}

TwLog& TwLog::operator<<(unsigned int d)
{
    m_logData << d;
    return *this;
}

TwLog& TwLog::operator<<(long d)
{
    m_logData << d;
    return *this;
}

TwLog& TwLog::operator<<(unsigned long d)
{
    m_logData << d;
    return *this;
}

TwLog& TwLog::operator<<( int64 d)
{
    m_logData<<d;
    return *this;
}

TwLog& TwLog::operator<<( uint64 d)
{
    m_logData<<d;
    return *this;
}

TwLog& TwLog::operator<<( const wchar_t* d )
{
    std::string str = wstringTostring(d);
    m_logData<<str;
    return *this;
}

TwLog& TwLog::operator<<( const std::wstring& d)
{
    std::string str = wstringTostring(d);
    m_logData<<str;
    return *this;
}

TwLog& TwLog::operator<<( const char* d )
{
    m_logData<<d;
    return *this;
}

TwLog& TwLog::operator<<( const std::string& d )
{
    m_logData<<d;
    return *this;
}

TwLog& TwLog::operator<<( void* d )
{
    m_logData<<(int)d;
    return *this;
}
