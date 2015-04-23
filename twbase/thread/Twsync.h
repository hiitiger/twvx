#pragma once


class TW_BASE_API TwWaitableEvent
{
public:

    TwWaitableEvent( bool manualReset = false, bool initialState = false)  //默认自动重置，未触发状态
        :m_handle(nullptr)
    {
        m_handle = CreateEvent(nullptr, manualReset, initialState, nullptr);
    }

    ~TwWaitableEvent()
    {
        CloseHandle(m_handle);
    }

    void set()
    {
        SetEvent(m_handle);
    }
    void reset()
    {
        ResetEvent(m_handle);
    }

    int wait(unsigned timeout = INFINITE)
    {
        DWORD ret = WaitForSingleObject (m_handle, timeout);  
        return ret == WAIT_OBJECT_0 ? 0 : -1;  
    }

    bool isSignal() 
    {
       return (wait(0) == 0);
    }

    HANDLE handle()
    {  
        return m_handle;
    }  
private:
    HANDLE m_handle;
};


class TW_BASE_API TwSemaphore
{
public:

    TwSemaphore( long initialCount = 0, long maximumCount = LONG_MAX)
        :m_handle(nullptr)
    {
        m_handle = CreateSemaphore(nullptr, initialCount, maximumCount, nullptr);
    }

    ~TwSemaphore(void)
    {
        CloseHandle(m_handle);
    }

    int release(long releaseCount = 1, long* previousCount = 0)
    {
        return ReleaseSemaphore(m_handle, releaseCount, previousCount);
    }

    int acquire(unsigned timeout = INFINITE)
    {
        DWORD ret = WaitForSingleObject (m_handle, timeout);  
        return ret == WAIT_OBJECT_0 ? 0 : -1;  
    }

    HANDLE handle()
    {  
        return m_handle;
    }  
private:
    HANDLE m_handle;
};

struct TW_BASE_API TwLockBase
{
    TW_NONCOPY(TwLockBase);
public:
    TwLockBase(){;}
    ~TwLockBase(){;}
    virtual int lock() = 0;
    virtual int unlock() = 0;
};

class TW_BASE_API TwMutex : public TwLockBase
{
public:

    TwMutex(bool initialOwner = false)
        :m_handle(nullptr)
    {
        m_handle = CreateMutex(nullptr, initialOwner, nullptr);
    }

    ~TwMutex()
    {
        CloseHandle(m_handle);
    }

    virtual int lock()
    {  
        DWORD ret = WaitForSingleObject (m_handle, INFINITE);
        return ret == WAIT_OBJECT_0 ? 0 : -1;
    } 

    virtual int unlock()
    {
        BOOL bret = ReleaseMutex (m_handle);
        return bret ? 0 : -1;
    }

    HANDLE handle()
    {  
        return m_handle;
    }  

private:
    HANDLE m_handle;
};


/*
    临界区 ，用户互斥
    避免同一线程多次进入，不可用于同步，注意不要造成死锁
*/
class TW_BASE_API TwLock : public TwLockBase
{
    
public:
    TwLock()
    {
        ZeroMemory( &_cs, sizeof(_cs) );
        InitializeCriticalSection(&_cs);
    }

    ~TwLock()
    {
        DeleteCriticalSection(&_cs);
    }

    virtual int lock()
    {
        EnterCriticalSection(&_cs);
        return 0;
    }

    virtual int unlock()
    {
        LeaveCriticalSection(&_cs);
        return 0;
    }

    bool tryLock()
    {
        return TryEnterCriticalSection(&_cs) ? true : false;
    }
private:
    CRITICAL_SECTION _cs;
};

class TW_BASE_API TwFakeLock  : public TwLockBase
{
    TwFakeLock() {;}
    virtual ~TwFakeLock() {;}

    virtual int lock() {return 0;}
    virtual int unlock() {return 0;}
};

class TW_BASE_API TwScopeLovkV1
{
public:
    TwScopeLovkV1(TwLockBase* lock)
        :m_lcok(lock)
    {
        m_lcok->lock();
    }
    ~TwScopeLovkV1()
    {
        m_lcok->unlock();
    }
private:
    TwLockBase* m_lcok;
};