#pragma once


class QX_BASE_API TextStream
{
public:
    TextStream()
    {
        m_data.reserve(512);
    }
    ~TextStream()
    {

    }

    const std::wstring& data() const { return m_data;  }

    TextStream& operator<<(bool d) { return (*this) << (d ? "true" : "false"); }
    TextStream& operator<<(char d) { return (*this) << (long)d;}
    TextStream& operator<<(short d) { return (*this) << (long)d;}
    TextStream& operator<<(unsigned short d) {return (*this) << (unsigned long)d;}
    TextStream& operator<<(int d) { return (*this) << (long)d;}
    TextStream& operator<<(unsigned int d) {return (*this) << (unsigned long)d;}

    TextStream& operator<<(long d)
    {
        wchar_t text[k_printBufferSize] = { 0 };
        _snwprintf_s(text, k_printBufferSize, _TRUNCATE, L"%d", d);
        m_data.append(text);
        return *this;
    }
    TextStream& operator<<(unsigned long d)
    {
        wchar_t text[k_printBufferSize] = { 0 };
        _snwprintf_s(text, k_printBufferSize, _TRUNCATE, L"%u", d);
        m_data.append(text);
        return *this;
    }
    TextStream& operator<<(int64_t d)
    {
        wchar_t text[k_printBufferSize] = { 0 };
        _snwprintf_s(text, k_printBufferSize, _TRUNCATE, L"%I64d", d);
        m_data.append(text);
        return *this;
    }
    TextStream& operator<<(uint64_t d)
    {
        wchar_t text[k_printBufferSize] = { 0 };
        _snwprintf_s(text, k_printBufferSize, _TRUNCATE, L"%I64u", d);
        m_data.append(text);
        return *this;
    }
    TextStream& operator<<(float d) {return (*this) << (double)d;}
    TextStream& operator<<(double d) 
    {
        wchar_t text[k_printBufferSize] = { 0 };
        _snwprintf_s(text, k_printBufferSize, _TRUNCATE, L"%0.8f", d);
        m_data.append(text);
        return *this;
    }
    TextStream& operator<<(const wchar_t* d){ m_data.append(d);return *this; }
    TextStream& operator<<(const std::wstring& d) { m_data.append(d);return *this; }
    TextStream& operator<<(const char* d){return (*this) << TwUtils::fromLocal8Bit(d);}
    TextStream& operator<<(const std::string& d) { return (*this) << TwUtils::fromLocal8Bit(d.c_str(), d.size());}

    TextStream& operator<<(const void* d)
    {
        wchar_t text[k_printBufferSize] = { 0 };
        _snwprintf_s(text, k_printBufferSize, _TRUNCATE, L"0x%08X", d);
        m_data.append(text);
        return *this;
    }

private:
    std::wstring m_data;
    static const int k_printBufferSize = 32;      
};
