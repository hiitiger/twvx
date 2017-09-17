#pragma once
#include "twthreadbase.h"

class TW_BASE_API TwThread : public TwThreadBase
{
    TW_DISABLE_COPY(TwThread);
public:
    TwThread();
    virtual ~TwThread();

    bool start();
    void stop();

    //unsafe, should only be called on create thead
    bool postFunctionUncheck(const std::function<void()>&);


    bool postFunction(const std::function<void()>&);

    bool isLoopRunning();

protected:
    virtual void threadMain();
    virtual void init();
    virtual void runloop();
    virtual void uninit();

private:
    // stack variable, live in threadLoop
    TwMessageLoop* m_loop = nullptr;

    std::thread::id m_createThreadId;
    std::atomic<bool> m_loopRunning = false;
    std::mutex m_loopLock;
};