#pragma once

class TW_BASE_API LogStream
{
public:
    LogStream(const char* mod, const char* file, const char* function, unsigned int line)
    : m_mod(mod)
    , m_file(file)
    , m_function(function)
    , m_line(line)
    {

    }
    ~LogStream();

    LogStream& operator<<(bool d) { m_stream << d; return *this; }
    LogStream& operator<<(char d) { m_stream << d; return *this; }
    LogStream& operator<<(short d) { m_stream << d; return *this; }
    LogStream& operator<<(unsigned short d) { m_stream << d; return *this; }
    LogStream& operator<<(int d) { m_stream << d; return *this; }
    LogStream& operator<<(unsigned int d) { m_stream << d; return *this; }
    LogStream& operator<<(long d) { m_stream << d; return *this; }
    LogStream& operator<<(unsigned long d) { m_stream << d; return *this; }
    LogStream& operator<<(int64_t d) { m_stream << d; return *this; }
    LogStream& operator<<(uint64_t d) { m_stream << d; return *this; }
    LogStream& operator<<(float d) { m_stream << d; return *this; }
    LogStream& operator<<(double d) { m_stream << d; return *this; }
    LogStream& operator<<(const char* d) { m_stream << d; return *this; }
    LogStream& operator<<(const std::string& d) { m_stream << d; return *this; }
    LogStream& operator<<(const wchar_t* d) { m_stream << d; return *this; }
    LogStream& operator<<(const std::wstring& d) { m_stream << d; return *this; }
    LogStream& operator<<(const void* d) { m_stream << d; return *this; }

private:
    TextStream m_stream;
    const char* m_mod;
    const char* m_file;
    const char* m_function;
    unsigned int m_line;
};



#define  TempLog  LogStream("tmp", __FILE__, __FUNCTION__, __LINE__)

#define TWLOG(mod)  LogStream(mod, __FILE__, __FUNCTION__, __LINE__)
