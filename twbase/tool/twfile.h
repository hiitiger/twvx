#pragma once

class QX_BASE_API TwFile
{
    TW_DISABLE_COPY(TwFile)
public:
    enum OpenMode {
        ReadOnly = 0x0001,
        WriteOnly = 0x0002,
        ReadWrite = ReadOnly | WriteOnly,
        Append = 0x0004,
        Truncate = 0x0008,
    };

    explicit TwFile(const std::wstring& filename);
    ~TwFile();

    const std::wstring& fileName();
    bool isValid() const;

    bool open(OpenMode openMode);
    void close();

    bool setSize(int64_t);
    int64_t size();

    int64_t pos() const;
    bool seek(int64_t offset);
    bool seekEnd();

    int64_t read(char* data, int64_t len);
    int64_t write(const char* data, int64_t len);

    static bool exist(const std::wstring& file);

private:
    std::wstring m_fileName;
    HANDLE m_file;
};

DEFINE_ENUM_FLAG_OPERATORS(TwFile::OpenMode);
