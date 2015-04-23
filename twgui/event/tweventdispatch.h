#pragma once

class TwEventPort;

class TW_GUI_API TwEventDispatcher
{
public: 
    TwEventDispatcher();
    virtual ~TwEventDispatcher();

    virtual void dispatchEvent(TwEvent* ev);

    static void sendEvent(TwEventPort*, TwEvent*);
    //!!will take owner of the event being posted.
    static void postEvent(TwEventPort*, TwEvent*);

};