#pragma once
#include "twobjecttraits.h"


typedef TwFakeLock Signal_SingleThread;
typedef TwLock     Signal_MutliThread;

#define TwSignalThreadPolicy Signal_MutliThread

//////////////////////////////////////////////////////////////////////////
class TwSignalConnectionBase
{
public:
    virtual ~TwSignalConnectionBase() {;}
    virtual void expire() = 0;
};

template<class ThreadPolicy = TwSignalThreadPolicy>
class TwSignalBase : public ThreadPolicy
{
public:
    virtual void disconnect_all() = 0;
};

#define  TWSIGNAL_COMMONCONNECTDISCONNECT \
    void connect(const std::shared_ptr<TwSignalConnection>& slot)\
    {\
        TwScopeLovkV1 lock(this);\
        auto it = std::find(m_slots.begin(), m_slots.end(), slot);\
        if (it != m_slots.end())\
        {\
            DAssert(false);\
            return;\
        }\
        slot->signal_ = this;\
        m_slots.push_back(slot);\
    }\
    void disconnect(const std::shared_ptr<TwSignalConnection>& slot)\
    {\
        TwScopeLovkV1 lock(this);\
        auto it = std::find(m_slots.begin(), m_slots.end(), slot);\
        if (it == m_slots.end())\
        {\
            DAssert(false);\
            return;\
        }\
        m_slots.erase(it);\
    }\
    virtual void disconnect_all()\
    {\
        TwScopeLovkV1 lock(this);\
        m_slots.clear();\
    }\
private:\
    void disconnect(TwSignalConnection* slot)\
    {\
        TwScopeLovkV1 lock(this);\
        auto it = std::find_if(m_slots.begin(), m_slots.end(),  [=](const std::shared_ptr<TwSignalConnection>& i)\
        {\
            return i.get() == slot;\
        });\
        if (it == m_slots.end())\
        {\
            DAssert(false);\
            return;\
        }\
        m_slots.erase(it);\
    }

template<class ThreadPolicy = TwSignalThreadPolicy>
class TwSignal0 : public TwSignalBase<ThreadPolicy>
{
public:
    class TwSignalConnection : public TwSignalConnectionBase
    {
    public:
        TwSignalConnection(TwSignal0* signal)
            :signal_(signal)
        {

        }
        virtual void expire()
        {
            signal_->disconnect(this);
            signal_ = nullptr;
        }

        virtual void onSignal(TwObject* sender) = 0;

        TwSignal0* signal_;
    };

    template<class O, class TObjHolder =TwHolderTraits<O>::TwHolder >
    class TwMemberSlot : public TwSignalConnection
    {
    public:
        typedef void (O::*Mfn)(TwObject* sender);

        TwMemberSlot(TwSignal0* signal, O* obj, Mfn fn)
            : TwSignalConnection(signal)
            , obj_(obj)
            , fn_(fn)
        {

        }

        virtual void onSignal(TwObject* sender)
        {
            DAssert(obj_);
            (getObj(obj_)->*fn_)(sender);
        }

        TObjHolder obj_;
        Mfn fn_;
    };

    class TwFunctionSlots : public TwSignalConnection
    {
    public:
        TwFunctionSlots(TwSignal0* signal, const std::function<void (TwObject*)>& func)
            : TwSignalConnection(signal)
            , func_(func)
        {

        }
        virtual void onSignal(TwObject* sender)
        {
            func_(sender);
        }

        std::function<void (TwObject*)> func_;
    };

    TwSignal0(TwObject* sender = nullptr)
        : m_sender(sender)
    {

    }
    virtual ~TwSignal0()
    {
        m_sender = nullptr;
        disconnect_all();
    }

    void emit()
    {
        TwScopeLovkV1 lock(this);
        for(auto it = m_slots.begin(); it != m_slots.end();)
        {
            auto itNext = it;
            ++itNext;

            (*it)->onSignal(m_sender);

            it = itNext;
        }
    }

    template<class O>
    std::shared_ptr<TwSignalConnection> connect(O* obj, void (O::*memberFunc)(TwObject* sender))
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwMemberSlot<O>(this, obj, memberFunc));
        connect(newSlot);
        return newSlot;
    }

    std::shared_ptr<TwSignalConnection> connect(const typename Identity<std::function<void (TwObject*)>>::type& func)
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwFunctionSlots(this, func));
        connect(newSlot);
        return newSlot;
    }

    TWSIGNAL_COMMONCONNECTDISCONNECT;

private:
    TwObject* m_sender;
    std::list<std::shared_ptr<TwSignalConnection> > m_slots;
};

template<class A1, class ThreadPolicy = TwSignalThreadPolicy>
class TwSignal1 : public TwSignalBase<ThreadPolicy>
{
public:
    typedef A1 Arg1_Type;

    class TwSignalConnection : public TwSignalConnectionBase
    {
    public:
        TwSignalConnection(TwSignal1* signal)
            :signal_(signal)
        {

        }
        virtual void expire()
        {
            signal_->disconnect(this);
            signal_ = nullptr;
        }

        virtual void onSignal(TwObject* sender, Arg1_Type arg1) = 0;

        TwSignal1* signal_;
    };

    template<class O, class TObjHolder =TwHolderTraits<O>::TwHolder >
    class TwMemberSlot : public TwSignalConnection
    {
    public:
        typedef void (O::*Mfn)(TwObject*, Arg1_Type);

        TwMemberSlot(TwSignal1* signal, O* obj, Mfn fn)
            : TwSignalConnection(signal)
            , obj_(obj)
            , fn_(fn)
        {

        }

        virtual void onSignal(TwObject* sender, Arg1_Type arg1)
        {
            DAssert(obj_);
            (getObj(obj_)->*fn_)(sender, arg1);
        }

        TObjHolder obj_;
        Mfn fn_;
    };

    class TwFunctionSlots : public TwSignalConnection
    {
    public:
        TwFunctionSlots(TwSignal1* signal, const std::function<void (TwObject*, Arg1_Type)>& func)
            : TwSignalConnection(signal)
            , func_(func)
        {

        }
        virtual void onSignal(TwObject* sender, Arg1_Type arg1)
        {
            func_(sender, arg1);
        }

        std::function<void (TwObject*, Arg1_Type)> func_;
    };

    TwSignal1(TwObject* sender = nullptr)
        : m_sender(sender)
    {

    }
    virtual ~TwSignal1()
    {
        m_sender = nullptr;
        disconnect_all();
    }

    void emit(Arg1_Type arg1)
    {
        TwScopeLovkV1 lock(this);
        for(auto it = m_slots.begin(); it != m_slots.end();)
        {
            auto itNext = it;
            ++itNext;

            (*it)->onSignal(m_sender, arg1);

            it = itNext;
        }
    }

    template<class O>
    std::shared_ptr<TwSignalConnection> connect(O* obj, void (O::*memberFunc)(TwObject*, Arg1_Type))
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwMemberSlot<O>(this, obj, memberFunc));
        connect(newSlot);
        return newSlot;
    }

    std::shared_ptr<TwSignalConnection> connect(const typename Identity<std::function<void(TwObject*, Arg1_Type)>>::type& func)
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwFunctionSlots(this, func));
        connect(newSlot);
        return newSlot;
    }

    TWSIGNAL_COMMONCONNECTDISCONNECT;
private:
    TwObject* m_sender;
    std::list<std::shared_ptr<TwSignalConnection> > m_slots;
};

template<class A1, class A2, class ThreadPolicy = TwSignalThreadPolicy>
class TwSignal2 : public TwSignalBase<ThreadPolicy>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;

    class TwSignalConnection : public TwSignalConnectionBase
    {
    public:
        TwSignalConnection(TwSignal2* signal)
            :signal_(signal)
        {

        }
        virtual void expire()
        {
            signal_->disconnect(this);
            signal_ = nullptr;
        }

        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2) = 0;

        TwSignal2* signal_;
    };

    template<class O, class TObjHolder =TwHolderTraits<O>::TwHolder >
    class TwMemberSlot : public TwSignalConnection
    {
    public:
        typedef void (O::*Mfn)(TwObject*, Arg1_Type, Arg2_Type);

        TwMemberSlot(TwSignal2* signal, O* obj, Mfn fn)
            : TwSignalConnection(signal)
            , obj_(obj)
            , fn_(fn)
        {

        }

        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2)
        {
            DAssert(obj_);
            (getObj(obj_)->*fn_)(sender, arg1, arg2);
        }

        TObjHolder obj_;
        Mfn fn_;
    };

    class TwFunctionSlots : public TwSignalConnection
    {
    public:
        TwFunctionSlots(TwSignal2* signal, const std::function<void (TwObject*, Arg1_Type, Arg2_Type)>& func)
            : TwSignalConnection(signal)
            , func_(func)
        {

        }
        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2)
        {
            func_(sender, arg1, arg2);
        }

        std::function<void (TwObject*, Arg1_Type, Arg2_Type)> func_;
    };

    TwSignal2(TwObject* sender = nullptr)
        : m_sender(sender)
    {

    }
    virtual ~TwSignal2()
    {
        m_sender = nullptr;
        disconnect_all();
    }

    void emit(Arg1_Type arg1, Arg2_Type arg2)
    {
        TwScopeLovkV1 lock(this);
        for(auto it = m_slots.begin(); it != m_slots.end();)
        {
            auto itNext = it;
            ++itNext;

            (*it)->onSignal(m_sender, arg1, arg2);

            it = itNext;
        }
    }

    template<class O>
    std::shared_ptr<TwSignalConnection> connect(O* obj, void (O::*memberFunc)(TwObject*, Arg1_Type, Arg2_Type))
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwMemberSlot<O>(this, obj, memberFunc));
        connect(newSlot);
        return newSlot;
    }

    std::shared_ptr<TwSignalConnection> connect(const typename Identity<std::function<void(TwObject*, Arg1_Type, Arg2_Type)>>::type& func)
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwFunctionSlots(this, func));
        connect(newSlot);
        return newSlot;
    }

   TWSIGNAL_COMMONCONNECTDISCONNECT;
private:
    TwObject* m_sender;
    std::list<std::shared_ptr<TwSignalConnection> > m_slots;
};

template<class A1, class A2, class A3, class ThreadPolicy = TwSignalThreadPolicy>
class TwSignal3 : public TwSignalBase<ThreadPolicy>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;

    class TwSignalConnection : public TwSignalConnectionBase
    {
    public:
        TwSignalConnection(TwSignal3* signal)
            :signal_(signal)
        {

        }
        virtual void expire()
        {
            signal_->disconnect(this);
            signal_ = nullptr;
        }

        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3) = 0;

        TwSignal3* signal_;
    };

    template<class O, class TObjHolder =TwHolderTraits<O>::TwHolder >
    class TwMemberSlot : public TwSignalConnection
    {
    public:
        typedef void (O::*Mfn)(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type);

        TwMemberSlot(TwSignal3* signal, O* obj, Mfn fn)
            : TwSignalConnection(signal)
            , obj_(obj)
            , fn_(fn)
        {

        }

        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3)
        {
            DAssert(obj_);
            (getObj(obj_)->*fn_)(sender, arg1, arg2, arg3);
        }

        TObjHolder obj_;
        Mfn fn_;
    };

    class TwFunctionSlots : public TwSignalConnection
    {
    public:
        TwFunctionSlots(TwSignal3* signal, const std::function<void (TwObject*, Arg1_Type, Arg2_Type, Arg3_Type)>& func)
            : TwSignalConnection(signal)
            , func_(func)
        {

        }
        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3)
        {
            func_(sender, arg1, arg2, arg3);
        }

        std::function<void (TwObject*, Arg1_Type, Arg2_Type, Arg3_Type)> func_;
    };

    TwSignal3(TwObject* sender = nullptr)
        : m_sender(sender)
    {

    }
    virtual ~TwSignal3()
    {
        m_sender = nullptr;
        disconnect_all();
    }

    void emit(Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3)
    {
        TwScopeLovkV1 lock(this);
        for(auto it = m_slots.begin(); it != m_slots.end();)
        {
            auto itNext = it;
            ++itNext;

            (*it)->onSignal(m_sender, arg1, arg2, arg3);

            it = itNext;
        }
    }

    template<class O>
    std::shared_ptr<TwSignalConnection> connect(O* obj, void (O::*memberFunc)(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type))
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwMemberSlot<O>(this, obj, memberFunc));
        connect(newSlot);
        return newSlot;
    }

    std::shared_ptr<TwSignalConnection> connect(const typename Identity<std::function<void(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type)>>::type& func)
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwFunctionSlots(this, func));
        connect(newSlot);
        return newSlot;
    }

    TWSIGNAL_COMMONCONNECTDISCONNECT;

private:
    TwObject* m_sender;
    std::list<std::shared_ptr<TwSignalConnection> > m_slots;
};

template<class A1, class A2, class A3, class A4, class ThreadPolicy = TwSignalThreadPolicy>
class TwSignal4 : public TwSignalBase<ThreadPolicy>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    typedef A4 Arg4_Type;

    class TwSignalConnection : public TwSignalConnectionBase
    {
    public:
        TwSignalConnection(TwSignal4* signal)
            :signal_(signal)
        {

        }
        virtual void expire()
        {
            signal_->disconnect(this);
            signal_ = nullptr;
        }

        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3, Arg4_Type arg4) = 0;

        TwSignal4* signal_;
    };

    template<class O, class TObjHolder =TwHolderTraits<O>::TwHolder >
    class TwMemberSlot : public TwSignalConnection
    {
    public:
        typedef void (O::*Mfn)(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type);

        TwMemberSlot(TwSignal4* signal, O* obj, Mfn fn)
            : TwSignalConnection(signal)
            , obj_(obj)
            , fn_(fn)
        {

        }

        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3, Arg4_Type arg4)
        {
            DAssert(obj_);
            (getObj(obj_)->*fn_)(sender, arg1, arg2, arg3, arg4);
        }

        TObjHolder obj_;
        Mfn fn_;
    };

    class TwFunctionSlots : public TwSignalConnection
    {
    public:
        TwFunctionSlots(TwSignal4* signal, const std::function<void (TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type)>& func)
            : TwSignalConnection(signal)
            , func_(func)
        {

        }
        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3, Arg4_Type arg4)
        {
            func_(sender, arg1, arg2, arg3, arg4);
        }

        std::function<void (TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type)> func_;
    };

    TwSignal4(TwObject* sender = nullptr)
        : m_sender(sender)
    {

    }
    virtual ~TwSignal4()
    {
        m_sender = nullptr;
        disconnect_all();
    }

    void emit(Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3, Arg4_Type arg4)
    {
        TwScopeLovkV1 lock(this);
        for(auto it = m_slots.begin(); it != m_slots.end();)
        {
            auto itNext = it;
            ++itNext;

            (*it)->onSignal(m_sender, arg1, arg2, arg3, arg4);

            it = itNext;
        }
    }

    template<class O>
    std::shared_ptr<TwSignalConnection> connect(O* obj, void (O::*memberFunc)(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type))
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwMemberSlot<O>(this, obj, memberFunc));
        connect(newSlot);
        return newSlot;
    }

    std::shared_ptr<TwSignalConnection> connect(const typename Identity<std::function<void(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type)>>::type& func)
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwFunctionSlots(this, func));
        connect(newSlot);
        return newSlot;
    }

    TWSIGNAL_COMMONCONNECTDISCONNECT;

private:
    TwObject* m_sender;
    std::list<std::shared_ptr<TwSignalConnection> > m_slots;
};

template<class A1, class A2, class A3, class A4, class A5, class ThreadPolicy = TwSignalThreadPolicy>
class TwSignal5 : public TwSignalBase<ThreadPolicy>
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    typedef A4 Arg4_Type;
    typedef A5 Arg5_Type;

    class TwSignalConnection : public TwSignalConnectionBase
    {
    public:
        TwSignalConnection(TwSignal5* signal)
            :signal_(signal)
        {

        }
        virtual void expire()
        {
            signal_->disconnect(this);
            signal_ = nullptr;
        }

        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3, Arg4_Type arg4, Arg5_Type arg5) = 0;

        TwSignal5* signal_;
    };

    template<class O, class TObjHolder =TwHolderTraits<O>::TwHolder >
    class TwMemberSlot : public TwSignalConnection
    {
    public:
        typedef void (O::*Mfn)(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type, Arg5_Type);

        TwMemberSlot(TwSignal5* signal, O* obj, Mfn fn)
            : TwSignalConnection(signal)
            , obj_(obj)
            , fn_(fn)
        {

        }

        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3, Arg4_Type arg4, Arg5_Type arg5)
        {
            DAssert(obj_);
            (getObj(obj_)->*fn_)(sender, arg1, arg2, arg3, arg4, arg5);
        }

        TObjHolder obj_;
        Mfn fn_;
    };

    class TwFunctionSlots : public TwSignalConnection
    {
    public:
        TwFunctionSlots(TwSignal5* signal, const std::function<void (TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type, Arg5_Type)>& func)
            : TwSignalConnection(signal)
            , func_(func)
        {

        }
        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3, Arg4_Type arg4, Arg5_Type arg5)
        {
            func_(sender, arg1, arg2, arg3, arg4, arg5);
        }

        std::function<void (TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type, Arg5_Type)> func_;
    };

    TwSignal5(TwObject* sender = nullptr)
        : m_sender(sender)
    {

    }
    virtual ~TwSignal5()
    {
        m_sender = nullptr;
        disconnect_all();
    }

    void emit(Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3, Arg4_Type arg4, Arg5_Type arg5)
    {
        TwScopeLovkV1 lock(this);
        for(auto it = m_slots.begin(); it != m_slots.end();)
        {
            auto itNext = it;
            ++itNext;

            (*it)->onSignal(m_sender, arg1, arg2, arg3, arg4, arg5);

            it = itNext;
        }
    }

    template<class O>
    std::shared_ptr<TwSignalConnection> connect(O* obj, void (O::*memberFunc)(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type, Arg5_Type))
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwMemberSlot<O>(this, obj, memberFunc));
        connect(newSlot);
        return newSlot;
    }

    std::shared_ptr<TwSignalConnection> connect(const typename Identity<std::function<void(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type, Arg5_Type)>>::type& func)
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwFunctionSlots(this, func));
        connect(newSlot);
        return newSlot;
    }

    TWSIGNAL_COMMONCONNECTDISCONNECT;

private:
    TwObject* m_sender;
    std::list<std::shared_ptr<TwSignalConnection> > m_slots;
};

template<class A1, class A2, class A3, class A4, class A5, class A6, class ThreadPolicy = TwSignalThreadPolicy>
class TwSignal6 : public TwSignalBase < ThreadPolicy >
{
public:
    typedef A1 Arg1_Type;
    typedef A2 Arg2_Type;
    typedef A3 Arg3_Type;
    typedef A4 Arg4_Type;
    typedef A5 Arg5_Type;
    typedef A6 Arg6_Type;

    class TwSignalConnection : public TwSignalConnectionBase
    {
    public:
        TwSignalConnection(TwSignal6* signal)
            :signal_(signal)
        {

        }
        virtual void expire()
        {
            signal_->disconnect(this);
            signal_ = nullptr;
        }

        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3, Arg4_Type arg4, Arg5_Type arg5, Arg6_Type arg6) = 0;

        TwSignal6* signal_;
    };

    template<class O, class TObjHolder = TwHolderTraits<O>::TwHolder >
    class TwMemberSlot : public TwSignalConnection
    {
    public:
        typedef void (O::*Mfn)(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type, Arg5_Type, Arg6_Type);

        TwMemberSlot(TwSignal6* signal, O* obj, Mfn fn)
            : TwSignalConnection(signal)
            , obj_(obj)
            , fn_(fn)
        {

        }

        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3, Arg4_Type arg4, Arg5_Type arg5, Arg6_Type arg6)
        {
            DAssert(obj_);
            (getObj(obj_)->*fn_)(sender, arg1, arg2, arg3, arg4, arg5, arg6);
        }

        TObjHolder obj_;
        Mfn fn_;
    };

    class TwFunctionSlots : public TwSignalConnection
    {
    public:
        TwFunctionSlots(TwSignal6* signal, const std::function<void(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type, Arg5_Type, Arg6_Type)>& func)
            : TwSignalConnection(signal)
            , func_(func)
        {

        }
        virtual void onSignal(TwObject* sender, Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3, Arg4_Type arg4, Arg5_Type arg5, Arg6_Type arg6)
        {
            func_(sender, arg1, arg2, arg3, arg4, arg5, arg6);
        }

        std::function<void(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type, Arg5_Type, Arg6_Type)> func_;
    };

    TwSignal6(TwObject* sender = nullptr)
        : m_sender(sender)
    {

    }
    virtual ~TwSignal6()
    {
        m_sender = nullptr;
        disconnect_all();
    }

    void emit(Arg1_Type arg1, Arg2_Type arg2, Arg3_Type arg3, Arg4_Type arg4, Arg5_Type arg5, Arg6_Type arg6)
    {
        TwScopeLovkV1 lock(this);
        for (auto it = m_slots.begin(); it != m_slots.end();)
        {
            auto itNext = it;
            ++itNext;

            (*it)->onSignal(m_sender, arg1, arg2, arg3, arg4, arg5, arg6);

            it = itNext;
        }
    }

    template<class O>
    std::shared_ptr<TwSignalConnection> connect(O* obj, void (O::*memberFunc)(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type, Arg5_Type, Arg6_Type))
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwMemberSlot<O>(this, obj, memberFunc));
        connect(newSlot);
        return newSlot;
    }

    std::shared_ptr<TwSignalConnection> connect(const typename Identity<std::function<void(TwObject*, Arg1_Type, Arg2_Type, Arg3_Type, Arg4_Type, Arg5_Type, Arg5_Type, Arg6_Type)>>::type& func)
    {
        std::shared_ptr<TwSignalConnection> newSlot(new TwFunctionSlots(this, func));
        connect(newSlot);
        return newSlot;
    }

    TWSIGNAL_COMMONCONNECTDISCONNECT;

private:
    TwObject* m_sender;
    std::list<std::shared_ptr<TwSignalConnection> > m_slots;
};
//////////////////////////////////////////////////////////////////////////

template<uint T, class Signature, class ThreadPolicy >
class TwSignalN ;

template<class Signature, class ThreadPolicy >
class TwSignalN<0,Signature, ThreadPolicy>
{
public:
    typedef TwSignal0<ThreadPolicy> type;
};

template<class Signature, class ThreadPolicy >
class TwSignalN<1,Signature, ThreadPolicy>
{
public:
    typedef TwSignal1<TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN(Signature, 1),ThreadPolicy> type;
};

template<class Signature, class ThreadPolicy >
class TwSignalN<2,Signature, ThreadPolicy>
{
public:
    typedef TwSignal2<TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN(Signature, 2),ThreadPolicy> type;
};

template<class Signature, class ThreadPolicy >
class TwSignalN<3,Signature, ThreadPolicy>
{
public:
    typedef TwSignal3<TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN(Signature, 3),ThreadPolicy> type;
};

template<class Signature, class ThreadPolicy >
class TwSignalN<4,Signature, ThreadPolicy>
{
public:
    typedef TwSignal4<TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN(Signature, 4),ThreadPolicy> type;
};

template<class Signature, class ThreadPolicy >
class TwSignalN<5,Signature, ThreadPolicy>
{
public:
    typedef TwSignal5<TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN(Signature, 5),ThreadPolicy> type;
};

template<class Signature, class ThreadPolicy >
class TwSignalN <6, Signature, ThreadPolicy >
{
public:
    typedef TwSignal6<TW_TEMPLATEARGFROMSIGNATUREANDARGCOUNT_GEN(Signature, 6), ThreadPolicy> type;
};


template<class Signature, class ThreadPolicy = TwSignalThreadPolicy>
class TwSignal : public TwSignalN<TwFunction<Signature>::ArgCount, Signature, ThreadPolicy>::type
{
public:
    typedef typename TwSignalN<TwFunction<Signature>::ArgCount, Signature, ThreadPolicy>::type baseType;
    TwSignal(TwObject* sender )
        : baseType(sender)
    {
        ;
    }
    virtual ~TwSignal()
    {
        ;
    }


};