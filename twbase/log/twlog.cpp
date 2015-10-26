#include "stable.h"
#include "twlog.h"
#include <process.h>
#include <time.h>


const int k_MAXLOGFILESIZE = 32 * 1024 * 1024;

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
        //__time64_t timet;
        SYSTEMTIME timet;
        std::wstring data;
    };


    DWORD m_threadId;
    std::wstring m_fileName;
    HANDLE m_writeEvent;
    HANDLE m_thread;
    TwLock m_logLock;
    std::deque<LogItem*> m_logItems;

    TwLog()
        : m_threadId(0)
        , m_writeEvent(NULL)
        , m_thread(NULL)
    {

        m_fileName = TwUtils::appDataPath();
        m_fileName.append(L"\\twvx\\");
        TwUtils::makeSureDirExist(m_fileName);

        WCHAR exePath[MAX_PATH];
        GetModuleFileNameW(NULL, exePath, MAX_PATH);
        WCHAR* name = wcsrchr(exePath, '\\');

        m_fileName.append(name);
        m_fileName.erase(m_fileName.find_last_of('.'));
        m_fileName.append(L".log");

        m_writeEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
        start();
    }
    ~TwLog()
    {
        ::PostThreadMessageW(m_threadId, WM_USER + 0X101, 0, 0);
        if (WAIT_TIMEOUT == WaitForSingleObject(m_thread, 5000))
        {
            TerminateThread(m_thread, -1);
        }
        CloseHandle(m_writeEvent);
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

    }

    void addLog(const char* mod, const char* file, const char* function, unsigned int line, const std::wstring& data)
    {
        LogItem* item = new LogItem;
        _snprintf_s(item->mod, _TRUNCATE, mod);
        _snprintf_s(item->file, _TRUNCATE, file);
        _snprintf_s(item->function, _TRUNCATE, function);
        item->line = line;
        GetLocalTime(&item->timet);
        item->data = data;
        TwScopeLovkV1 lock(&m_logLock);
        m_logItems.push_back(item);
        SetEvent(m_writeEvent);
    }

private:
    void _writeLog(TwFile& logFile)
    {
        std::deque<LogItem*> logItems;
        {
            TwScopeLovkV1 lock(&m_logLock);
            logItems.swap(m_logItems);
        }

        if (logFile.size() > k_MAXLOGFILESIZE)
        {
            logFile.setSize(0);
        }

        for (std::deque<LogItem*>::iterator it = logItems.begin(); it != logItems.end(); ++it)
        {
            LogItem* item = *it;

            char timesz[128] = { 0 };
            sprintf_s(timesz, "%d-%d-%d %d:%d:%d:%d ", item->timet.wYear, item->timet.wMonth, item->timet.wDay, item->timet.wHour, item->timet.wMinute, item->timet.wSecond, item->timet.wMilliseconds);
            logFile.write(timesz, strlen(timesz));

            size_t logSize = item->data.size();
            char* log = (char*)malloc(sizeof(char) * 320 + logSize + 1);
            memset(log, 0, sizeof(char) * 320 + logSize + 1);
            int writeSize = sprintf_s(log, sizeof(char) * 256 + 32 + logSize + 1, "mod:%s,file:%s,function:%s,line:%d,log:%s\r\n", item->mod, item->file, item->function, item->line, TwUtils::toUtf8(item->data.c_str(), static_cast<int>(item->data.size())).c_str());
            logFile.write(log, writeSize);
            free(log);

            delete item;
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
        logFile.open(TwFile::ReadWrite|TwFile::Append);

        bool running = true;
        while (running)
        {
            DWORD wait = MsgWaitForMultipleObjects(1, &m_writeEvent, FALSE, 5000, QS_ALLINPUT);
            if (wait == WAIT_OBJECT_0 || wait == WAIT_TIMEOUT)
            {
                _writeLog(logFile);
            }
            else if(wait == WAIT_OBJECT_0 + 1)
            {
                MSG msg;
                while (GetMessageW(&msg, NULL, 0, 0))
                {
                    if (msg.message == WM_USER + 0X101)
                    {
                        _writeLog(logFile);
                        SYSTEMTIME timet;
                        ::GetLocalTime(&timet);
                        char timesz[128] = { 0 };
                        sprintf_s(timesz, "%d-%d-%d %d:%d:%d:%d End...", timet.wYear, timet.wMonth, timet.wDay, timet.wHour, timet.wMinute, timet.wSecond, timet.wMilliseconds);
                        logFile.write(timesz, strlen(timesz));

                        running = false;
                        break;
                    }

                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
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
    TwLog::instance()->addLog(m_mod, m_file, m_function, m_line, m_stream.data());
}
