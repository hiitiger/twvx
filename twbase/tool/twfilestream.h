#pragma once


class TW_BASE_API TwFileStream
{
public:
    explicit TwFileStream(const std::wstring& filename);
    ~TwFileStream();

    const std::wstring& fileName();

    bool open(Tw::OpenMode openmode);
    void close();

    bool isValid() const;

    int64 size();

    int64 pos() const;
    void seek(int64 offset);

    int64 read(void* data, int64 len);
    int64 write(void* data, int64 len);

private:
    std::wstring m_fileName;
    HANDLE m_file;
};