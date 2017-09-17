#pragma once


class TW_BASE_API TwMessageLoop
{
    TW_DISABLE_COPY(TwMessageLoop);
    friend struct QuitTask;

public:

    class WrapTask
    {
 
    public:
        WrapTask(const std::function<void()>& func, const TwTimeTick& runTime)
            : func_(func), post(TwTimeTick::now()), run(runTime)
        {

        }

        WrapTask(std::function<void()>&& func, const TwTimeTick& runTime)
            : func_(std::move(func)), post(TwTimeTick::now()), run(runTime)
        {

        }

        WrapTask(const std::function<void()>& func)
            : func_(func), post(TwTimeTick::now())
        {

        }

        WrapTask(std::function<void()>&& func)
            : func_(func), post(TwTimeTick::now())
        {

        }

        WrapTask(WrapTask&&) = default;

        WrapTask& operator=(WrapTask&&) = default;

        bool operator < (const WrapTask& other) const
        {
            if (run < other.run)
            {
                return false;
            }
            if (run > other.run)
            {
                return true;
            }
            return seq_ > other.seq_;
        }


        void invoke()
        {
            func_();
        }

        std::function<void()> func_;

        TwTimeTick post;

        TwTimeTick run;

        int seq_ = 0;
    };

    TwMessageLoop();
    virtual ~TwMessageLoop();

    static TwMessageLoop* current();

    bool isRunning() const;

    //async quit by quittask 
    void quit(int retCode = 0);
    //just set quit flag, it will prevent remained tasks from running.
    void quitQuick(int retCode = 0);

    void postFunction(const std::function<void()>&);
    void postDelayed(const  std::function<void()>& func, int milliSeconds);

    void awake();
    void awakeTask();

    virtual int run();
    virtual void runOnce();
    virtual void tryIdleWait(unsigned int milliSeconds = UINT32_MAX);
    virtual void idleWait(unsigned int milliSeconds);

protected:
    void runTaskQueue();
    void runDelayQueue();
    virtual void processSystemMessage();

private:
    void add(WrapTask&& task);

    void initNativeMsgWindow();
    static LRESULT CALLBACK LoopWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    std::deque<WrapTask> lockSwapTaskQueue();

private:

    std::atomic<bool> m_running = true;
    std::atomic<bool> m_hasWorkTask = false;
    int m_retCode;
    HWND m_msgWindow;

    TwLock m_taskLock;
    std::atomic<int> taskSeq_ = 0;
    std::deque<WrapTask>  m_taskQueue;
    std::priority_queue<WrapTask> m_delayQueue;
    TwTimeTick m_nextDelay;

};