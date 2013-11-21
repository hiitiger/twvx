#include "stable.h"
#include "TwAsyncTaskService.h"
#include "twsingleton.h"
#include "twguiinternaldefine.h"
#include "twapp.h"

TwDefine_SingleTon(TwAsyncTaskService,_twAsyncTaskService);

TwAsyncTaskService* twAsyncTaskService()
{
    return _twAsyncTaskService();
}


TwAsyncTaskService::TwAsyncTaskService(void)
{
    _init();
}

TwAsyncTaskService::~TwAsyncTaskService(void)
{
    if (twApp())
    {
        twApp()->sigTwAppEvent.disconnect(this);
    }
    _clear();
}

void TwAsyncTaskService::appendTask( IAsyncTask* task )
{
    TwScopeLovkV1 lock(&m_taskLock);
    m_taskQueue.push_back(task);
    twApp()->postAppEvent(TwE::EventIdAyncsTask);
}

void TwAsyncTaskService::excute()
{
    std::vector<IAsyncTask*> tasks;
    {
        TwScopeLovkV1 lock(&m_taskLock);
        tasks = m_taskQueue;
        m_taskQueue.clear();
    }

    for(std::vector<IAsyncTask*>::iterator it = tasks.begin(); it != tasks.end(); ++it)
    {
        (*it)->run();    
        (*it)->finish();    
        delete *it;
        *it = NULL;
    }
}

void TwAsyncTaskService::_init()
{
    twApp()->registerInterEventId(TwE::EventIdAyncsTask);
    twApp()->sigTwAppEvent.connect(this,&TwAsyncTaskService::onAsyncTaskEvent);
}

void TwAsyncTaskService::onAsyncTaskEvent( int id )
{
    if (id == TwE::EventIdAyncsTask)
    {
        excute();
    }
}

void TwAsyncTaskService::_clear()
{
    if (m_taskQueue.size() > 0) //should not happen
    {
        for(std::vector<IAsyncTask*>::iterator it = m_taskQueue.begin(); it != m_taskQueue.end(); ++it)
        {
            delete *it;
            *it = NULL;
        }
    }
}
