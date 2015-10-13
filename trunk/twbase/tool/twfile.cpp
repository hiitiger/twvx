#include "stable.h"
#include "twfile.h"


TwFile::TwFile(const std::wstring& filename)
    : m_fileName(filename)
    , m_file(INVALID_HANDLE_VALUE)
{

}

TwFile::~TwFile()
{
    close();
}

const std::wstring& TwFile::fileName()
{
    return m_fileName;
}

bool TwFile::isValid() const
{
    return m_file != INVALID_HANDLE_VALUE;
}

bool TwFile::open(OpenMode openMode)
{
    if (m_fileName.empty())
    {
        return false;
    }

    DWORD accessRight = 0;
    if (openMode & TwFile::ReadOnly)
    {
        accessRight |= GENERIC_READ;
    }
    if (openMode & TwFile::WriteOnly)
    {
        accessRight |= GENERIC_WRITE;
    }

    SECURITY_ATTRIBUTES securityAtts = { sizeof(SECURITY_ATTRIBUTES), NULL, FALSE };

    DWORD creationDisp = (openMode & TwFile::WriteOnly) ? OPEN_ALWAYS : OPEN_EXISTING;

    m_file = ::CreateFileW(m_fileName.c_str(), accessRight, FILE_SHARE_READ | FILE_SHARE_WRITE, &securityAtts, creationDisp, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (m_file == INVALID_HANDLE_VALUE)
    {
        DWORD err = GetLastError();
        (void)err;
        return false;
    }

    if (openMode & TwFile::Truncate)
    {
        setSize(0);
    }

    if (openMode & TwFile::Append)
    {
        seekEnd();
    }

    return true;
}

void TwFile::close()
{
    if (m_file != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_file);
        m_file = INVALID_HANDLE_VALUE;
    }
}

bool TwFile::setSize(int64_t sz)
{
    if (m_file == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    
    int64_t currentPos = pos();

    if (seek(sz) && ::SetEndOfFile(m_file))
    {
        seek(twMin(currentPos, sz));
        return true;
    }
    
    seek(currentPos);
    return false;
}

int64_t TwFile::size()
{
    WIN32_FILE_ATTRIBUTE_DATA attributes = { 0 };
    if (::GetFileAttributesExW(fileName().c_str(), GetFileExInfoStandard, (void*)&attributes))
    {
        return (((int64_t)attributes.nFileSizeHigh) << 32) | attributes.nFileSizeLow;
    }
    return -1;
}

int64_t TwFile::pos() const
{
    if (m_file == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    LARGE_INTEGER currentPos;
    LARGE_INTEGER move;
    move.QuadPart = 0;
    if (::SetFilePointerEx(m_file, move, &currentPos, FILE_CURRENT))
    {
        return static_cast<int64_t>(currentPos.QuadPart);
    }
    return -1;
}

bool TwFile::seek(int64_t offset)
{
    if (m_file == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    LARGE_INTEGER currentPos;
    LARGE_INTEGER move;
    move.QuadPart = offset;
    if (!::SetFilePointerEx(m_file, move, &currentPos, FILE_BEGIN))
    {
        DWORD err = ::GetLastError();
        (void)err;
        return false;
    }

    return true;
}

bool TwFile::seekEnd()
{
    if (m_file == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    return INVALID_SET_FILE_POINTER != ::SetFilePointer(m_file, 0, 0, FILE_END);
}

int64_t TwFile::read(char* data, int64_t len)
{
    if (m_file == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    static const DWORD maxBlockSize = 32 * 1024 * 1024;
    int64_t totalRead = 0;
    DWORD bytesToRead = DWORD(len);

    do {
        DWORD blockSize = twMin(bytesToRead, maxBlockSize);
        DWORD bytesRead = 0;

        if (!::ReadFile(m_file, data + totalRead, blockSize, &bytesRead, nullptr))
        {
            if (totalRead == 0)
            {
                DWORD err = ::GetLastError();
                (void)err;
                return -1;
            }
            else
            {
                break;
            }
        }

        if (bytesRead == 0)
        {
            break;
        }

        totalRead += bytesRead;
        bytesToRead -= bytesRead;
    } while (totalRead < len);

    return totalRead;

}

int64_t TwFile::write(const char* data, int64_t len)
{
    if (m_file == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    static const DWORD maxBlockSize = 32 * 1024 * 1024;
    int64_t totalWritten = 0;
    DWORD bytesToWrite = DWORD(len);
    do
    {
        DWORD blockSize = twMin(bytesToWrite, maxBlockSize);
        DWORD bytesWritten = 0;

        if (!::WriteFile(m_file, data + totalWritten, blockSize, &bytesWritten, nullptr))
        {
            if (totalWritten == 0)
            {
                DWORD err = ::GetLastError();
                (void)err;
                return -1;
            }
            else
            {
                break;
            }
        }

        if (bytesWritten == 0)
        {
            break;
        }

        totalWritten += bytesWritten;
        bytesToWrite -= bytesWritten;

    } while (totalWritten < len);

    return totalWritten;
}

bool TwFile::exist(const std::wstring& file)
{
    WIN32_FILE_ATTRIBUTE_DATA findData;
    return 0 != ::GetFileAttributesEx(file.c_str(), GetFileExInfoStandard, &findData);
}
