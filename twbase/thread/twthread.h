#pragma once
#include "twthreadbase.h"

class TW_BASE_API TwThread : public TwThreadBase
{
    TW_DISABLE_COPY(TwThread);
public:
    TwThread();
    virtual ~TwThread();

    //just set quit flag, it will prevent remained tasks from running.
    void stopQuick();

    bool postTask(TwTask*);
    bool postFunction(const std::function<void()>&);

protected:
    virtual void threadMain();
    virtual void init() {;}
    virtual void threadLoop() { m_loop->run(nullptr); }
    virtual void uninit() {;}

private:
    // stack variable, live in threadLoop
    TwMessageLoop* m_loop;

};