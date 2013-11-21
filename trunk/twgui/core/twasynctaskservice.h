#pragma once
#include <vector>
#include "tools/xsigslot.h"
#include "twsync.h"
#include "iasynctask.h"

class TwAsyncTaskService : public sigslot::has_slots<>
{
public:
    TwAsyncTaskService(void);
    ~TwAsyncTaskService(void);
    
    void appendTask(IAsyncTask*);
    void excute();
  //  void appendQueuedItem();

public TwSlot:
    void onAsyncTaskEvent(int id);

private: 
    void _init();
    void _clear();

private:
    TwLock m_taskLock;
    std::vector<IAsyncTask*> m_taskQueue;
};

 TwAsyncTaskService* twAsyncTaskService();