#include "stable.h"
#include "tweventport.h"
#include "tweventdispatch.h"


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
    TwMessageLoop::current()->postFunction([weakObj = TwWeakPtr<TwEventPort>(obj),  ev = std::shared_ptr<TwEvent>(ev)]{
        if (weakObj)
        {
            weakObj->processEvent(ev.get());
        }
    });
}

void TwEventDispatcher::dispatchEvent( TwEvent* ev )
{
    TW_UNUSED(ev);
}
