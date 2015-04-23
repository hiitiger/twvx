#pragma once

class TwEvent;

class TW_GUI_API TwEventPort : public TwObject
{
    TW_CLASS_DECLARE(TwEventPort);
public:
    explicit TwEventPort(TwObject* parent = nullptr);
    virtual ~TwEventPort();

    void installEventHook(TwEventPort*);
    void removeEventHook(TwEventPort*);

    bool processEvent(TwEvent* e);

protected:
    bool filterEventHooks(TwEvent* e);

    virtual void onEvent(TwEvent* );

    virtual bool hookHandleEvent(TwEventPort*,TwEvent*);

private:
    typedef std::list<TwWeakPtr<TwEventPort> > EventHooksList;
    EventHooksList m_eventHooks;
};

TW_CLASS_REGISTER(TwEventPort);
