#pragma once

class TwBaseService 
{
    TW_DISABLE_COPY(TwBaseService);
    friend class TwMessageLoop;
    friend class TimerSink;
public:
    TwBaseService(TwMessageLoop* loop);
    ~TwBaseService();

public:
    void onServiceMessage(int id, void* arg);

    //
    void asyncDeleteObject(TwObject* obj);

    //
    int setTimer( int elapseMSecs,TwTimer* timer );
    bool killTimer( int timerId );
    void handlerTimerEvent(int timerId);

    static void startTimer(int milliSeconds, const std::function<bool()>& callback);
    static void startSingleShotTimer(int milliSeconds, const std::function<bool()>& callback);

private:
    int nextTimerId();
    void addTimerSink(TimerSink*);
    void eraseTimerSink(TimerSink*);

private:
    TwMessageLoop* m_loop;
    //timer service
    int m_nextTimerId;
    std::map<int,TwTimer*> m_timerQueue;
    std::set<std::shared_ptr<TimerSink> > m_timerSinks;
};

TwBaseService* twBaseService();


