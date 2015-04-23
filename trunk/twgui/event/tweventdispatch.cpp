#include "stable.h"
#include "tweventport.h"
#include "tweventdispatch.h"

struct EventPostTask : public TwTask
{
    EventPostTask(TwEventPort* obj, TwEvent* ev)
        : obj_(obj)
        , ev_(ev)
    {;}

    virtual void run()
    {
        if (obj_)
        {
            obj_->processEvent(ev_.get());
        }
    }

    TwWeakPtr<TwEventPort> obj_;
    std::shared_ptr<TwEvent> ev_;
};

TwEventDispatcher::TwEventDispatcher()
{

}

TwEventDispatcher::~TwEventDispatcher()
{

}

void TwEventDispatcher::sendEvent( TwEventPort* obj, TwEvent* ev)
{
    obj->processEvent(ev);
}

void TwEventDispatcher::postEvent( TwEventPort* obj, TwEvent* ev)
{
    TwMessageLoop::currentLoop()->postTask(new EventPostTask(obj, ev));
}

void TwEventDispatcher::dispatchEvent( TwEvent* ev )
{
    TW_UNUSED(ev);
}
