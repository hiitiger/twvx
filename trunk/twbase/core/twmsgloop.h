#pragma once

#include "twscoped_ptr.h"

struct TwTask;
class TwBaseService;

/*
    * TwMessageLoop
    ** Always use TwMessageLoop or it's subclass if possible!
    ** Task will be executed during the loop, not in the callback WndPoc !
    ** If u use another message loop or a system modal dialog, it will prevent the task from executing!
   
*/
class TW_BASE_API TwMessageLoop
{
    TW_DISABLE_COPY(TwMessageLoop);
    friend class TwBaseService;
    friend struct QuitTask;

public:

    struct IDispatcher
    {
        virtual bool dispatchWinMessage(const MSG& msg) = 0;
    };

    TwMessageLoop();
    virtual ~TwMessageLoop();

    static TwMessageLoop* currentLoop();
    TwBaseService* baseService();

    int run(IDispatcher* disp = nullptr);
    bool isRunning() const;

    //async quit by quittask 
    void quit(int retCode = 0);
    //just set quit flag, it will prevent remained tasks from running.
    void quitQuick(int retCode = 0);

    void postTask(TwTask*);
    void postTask(const std::shared_ptr<TwTask>&);
    void postFunction(const std::function<void()>&);

    void awake();
    void awakeTask();

protected:
    bool runLoop(IDispatcher* disp = nullptr);

    /*virtual*/ bool processMessage(IDispatcher* disp);
    /*virtual*/ bool runTask();
    /*virtual*/ void waitWork();

private:
    void initNativeMsgWindow();
    static LRESULT CALLBACK LoopWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    bool loadWorkTask();

private:
    typedef std::queue<std::shared_ptr<TwTask> > TaskQueue;

    bool* m_running;
    volatile bool m_hasPendingTask;
    int m_retCode;
    HWND m_msgWindow;

    TwLock m_taskLock;
    TaskQueue m_pendingTaskQueue;
   // TaskQueue m_workTaskQueue;
    std::shared_ptr<TwTask> m_currentTask;

    TwScopedPtr<TwBaseService> m_baseservice;
};