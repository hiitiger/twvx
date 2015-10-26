#pragma once

class TW_BASE_API TwThreadBase
{
    TW_NONCOPY(TwThreadBase);
public:
    TwThreadBase();
    virtual ~TwThreadBase();

    //wait until thread start or fail.
    bool start();
    //wait until thread stop.
    virtual void stop();

    uint32 threadId() const { return m_threadId;}
    bool isRunning() const { return m_threadHandle != nullptr;}

protected:
    virtual void threadMain() {m_startEvent->set(); threadLoop();}
    virtual void threadLoop() {;}

private:
    static unsigned int __stdcall _threadProc(void*);

protected:
    static const uint32 k_InvalidId = 0;

    uint32 m_threadId;
    HANDLE m_threadHandle;
    TwScopedPtr<TwWaitableEvent> m_startEvent;
};