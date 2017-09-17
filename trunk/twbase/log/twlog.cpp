#include "stable.h"
#include "twlog.h"
#include <process.h>
#include "tool/twfile.h"
#include "base/datetime.h"

const int k_MAXLOGFILESIZE = 32 * 1024 * 1024;
static bool k_logQuit = false;

class TwLog
{
public:
    struct LogItem
    {
        LogItem()
        {
            memset(mod, 0, sizeof(char) * 64);
            memset(file, 0, sizeof(char) * 128);
            memset(function, 0, sizeof(char) * 128);
        }
        char mod[64];
        char file[128];
        char function[128];
        unsigned int line;
        TwDateTime datetime;
        std::wstring data;
    };


    DWORD m_threadId;
    std::wstring m_fileName;
    TwWaitableEvent m_writeEvent;
    HANDLE m_thread;
    std::mutex m_logLock;
    std::deque<std::unique_ptr<LogItem>> m_logItems;

    TwLog()
        : m_threadId(0)
        , m_writeEvent()
        , m_thread(NULL)
    {

        m_fileName = TwUtils::appDataPath();
        m_fileName.append(L"\\log\\");
        TwUtils::makeSureDirExist(m_fileName);

        m_fileName.append(TwUtils::appProcessName());
        m_fileName.erase(m_fileName.find_last_of('.'));
        m_fileName.append(L".log");

        start();
    }
    ~TwLog()
    {
        k_logQuit = true;
        m_writeEvent.set();

        if (WAIT_TIMEOUT == WaitForSingleObject(m_thread, 5000))
        {
            TerminateThread(m_thread, -1);
        }
        CloseHandle(m_thread);
    }

    static TwLog*instance()
    {
        static TwLog logInstance;
        return &logInstance;
    }

    void start()
    {
        m_thread = (HANDLE)_beginthreadex(NULL, 0, logThread, (void*)this, 0, (unsigned int*)&m_threadId);

        addLog("MOD_LOGGER", __FILE__, __FUNCTION__, __LINE__, L"Start...");
    }

    void addLog(const char* mod, const char* file, const char* function, unsigned int line, const std::wstring& data)
    {
        LogItem* item = new LogItem;
        _snprintf_s(item->mod, _TRUNCATE, mod);
        _snprintf_s(item->file, _TRUNCATE, file);
        _snprintf_s(item->function, _TRUNCATE, function);
        item->line = line;
        item->datetime = TwDateTime::now();
        item->data = data;

        std::lock_guard<std::mutex> lock(m_logLock);
        m_logItems.push_back(std::unique_ptr<LogItem>(item));
        m_writeEvent.set();
    }

private:
    void _writeLog(TwFile& logFile)
    {
        std::deque<std::unique_ptr<LogItem>> logItems;
        {
            std::lock_guard<std::mutex> lock(m_logLock);
            logItems.swap(m_logItems);
        }

        if (logFile.size() > k_MAXLOGFILESIZE)
        {
            logFile.setSize(0);
        }

        const size_t HEADER_SIZE = 512;
        char log[HEADER_SIZE] = { 0 };

        for (auto it = logItems.begin(); it != logItems.end(); ++it)
        {
            auto item = std::move(*it);

            std::string timesz = toString(item->datetime);
            logFile.write(timesz.data(), timesz.size());

            memset(log, 0, sizeof(char) * HEADER_SIZE);
            int headerSize = sprintf_s(log, HEADER_SIZE, " mod:%s,file:%s,function:%s,line:%d,log:", item->mod, item->file, item->function, item->line);
            logFile.write(log, headerSize);

            std::string content = TwUtils::toUtf8(item->data.c_str(), static_cast<int>(item->data.size()));
            logFile.write(content.c_str(), content.size());
            logFile.write("\r\n", 2);
        }
    }

    static  unsigned int __stdcall logThread(void* p)
    {
        ((TwLog*)p)->_logThread();
        return 0;
    }

    void _logThread()
    {
        TwFile logFile(m_fileName.c_str());
        logFile.open(TwFile::ReadWrite | TwFile::Append);
        if (logFile.size() > k_MAXLOGFILESIZE)
        {
            logFile.close();
            std::wstring bak = m_fileName + L".bak";
            if (TwFile::exist(bak))
            {
                DeleteFileW(bak.c_str());
            }
            MoveFileW(m_fileName.c_str(), bak.c_str());
            logFile.open(TwFile::ReadWrite | TwFile::Truncate);
        }

        bool running = true;
        while (running)
        {
            TwWaitableEvent::State s = m_writeEvent.wait(5000);
            if (s == TwWaitableEvent::Signal || s == TwWaitableEvent::Timeout)
            {
                _writeLog(logFile);
                if (k_logQuit)
                {
                    std::string timesz = toString(TwDateTime::now());
                    logFile.write(timesz.data(), timesz.size());

                    std::string end = " End..\r\n";
                    logFile.write(end.data(), end.size());
                    running = false;
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
};


LogStream::~LogStream()
{
    if (!k_logQuit)
    {
        TwLog::instance()->addLog(m_mod, m_file, m_function, m_line, m_stream.data());
    }
}
