#include "stable.h"
#include "twfilestream.h"


TwFileStream::TwFileStream(const std::wstring& filename)
    : m_fileName(filename)
    , m_file(INVALID_HANDLE_VALUE)
{

}

TwFileStream::~TwFileStream()
{
    close();
}

const std::wstring& TwFileStream::fileName()
{
    return m_fileName;
}

bool TwFileStream::open(Tw::OpenMode openmode)
{
    if (m_fileName.empty())
    {
        return false;
    }

    DWORD accessRight;
    if (openmode & Tw::ReadOnly)
    {
        accessRight |= GENERIC_READ;
    }
    if (openmode & Tw::WriteOnly)
    {
        accessRight |= GENERIC_WRITE;
    }

    DWORD creationDisp = OPEN_ALWAYS;

    if (openmode & Tw::Truncate)
    {
        creationDisp = CREATE_ALWAYS;
    }
    m_file = ::CreateFile(m_fileName.c_str(), accessRight, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, creationDisp, FILE_ATTRIBUTE_NORMAL, nullptr);

    return m_file != INVALID_HANDLE_VALUE;
}

void TwFileStream::close()
{
    if (m_file != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_file);
        m_file = INVALID_HANDLE_VALUE;
    }
}

bool TwFileStream::isValid() const
{
    return m_file != INVALID_HANDLE_VALUE;
}

int64 TwFileStream::size()
{
    WIN32_FILE_ATTRIBUTE_DATA attributes = { 0 };
    if (::GetFileAttributesExW(fileName().c_str(), GetFileExInfoStandard, (void*)&attributes))
    {
        return (((int64)attributes.nFileSizeHigh) << 32) | attributes.nFileSizeLow;
    }
    return -1;
}

int64 TwFileStream::pos() const
{
    if (m_file != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER currentPos;
        LARGE_INTEGER move;
        move.QuadPart = 0;
        if (::SetFilePointerEx(m_file, move, &currentPos, FILE_CURRENT))
        {
            return static_cast<int64>(currentPos.QuadPart);
        }
    }
    return -1;
}

void TwFileStream::seek(int64 offset)
{
    if (m_file != INVALID_HANDLE_VALUE)
    {
        if (pos() + offset < size())
        {
            ::SetFilePointer(m_file, 0, 0, FILE_END);
        }
        else if (pos() + offset < 0)
        {
            ::SetFilePointer(m_file, 0, 0, FILE_BEGIN);
        }
        else
        {
            LARGE_INTEGER move;
            move.QuadPart = 0;
            ::SetFilePointerEx(m_file, move, nullptr, FILE_CURRENT);
        }
    }
}

int64 TwFileStream::read(void* data, int64 len)
{
    if (m_file != INVALID_HANDLE_VALUE)
    {
        DWORD readBytes = 0;
        if (::ReadFile(m_file, data, static_cast<DWORD>(len), &readBytes, nullptr))
        {
            return  static_cast<int64>(readBytes);
        }
    }
    return -1;
}

int64 TwFileStream::write(void* data, int64 len)
{
    if (m_file != INVALID_HANDLE_VALUE)
    {
        DWORD writeBytes = 0;
        if (::WriteFile(m_file, data, static_cast<DWORD>(len), &writeBytes, nullptr))
        {
            return static_cast<int64>(writeBytes);
        }
    }
    return -1;
}
