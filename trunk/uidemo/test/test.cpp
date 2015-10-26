#include "stable.h"
#include "test.h"


TwTestFun(TestObject)
{
    TwWindow* ww = new TwWindow;
    ww->setTitle(L"TestDelete");
    ww->windowId();
    TwObject* obj = ww;
    std::wstring ty = obj->classInfo()->m_className;
    TempLog<<ty;

    TwWindow* window = twObject_cast<TwWindow*>(obj);
    DAssert(window);
    std::wstring ty2 = window->classInfo()->m_className;
    TempLog<<ty2;

    TwObject haha;
    TwWindow* xwindow = twObject_cast<TwWindow*>(&haha);
    TW_UNUSED(xwindow);
    DAssert(!xwindow);

    ww->deleteLater();
    return true;
}

TwTestFun(TestDelete)
{
     TwWindow* ww = new TwWindow;
     ww->setTitle(L"TestDelete");
     ww->show();
     ww->deleteLater();
    return true;
}
TwTestFun(TestWeakPtr)
{
    TwWindow* ww = new TwWindow;
    TwWeakPtr<TwWindow> window(ww);

    window->setName(L"xxxx");
    delete ww;
    if (window)
    {
        DAssert_M(false,"TestWeakPtr: false!!");
    }

    return !window;
}

TwTestFun(TestTimer)
{
    TwTimer::startTimer(1000, [] {
        TempLog<<"timer ticks";
        return true;
    });

    TwTimer::startSingleShotTimer(100, [] {
        TempLog<<"timer ticks single Shot";
        return true;
    });
    return true;
}


struct  TestSignalOSlot : public TwObject
{
    explicit TestSignalOSlot(TwWindow* w)
    :w_(w)
    {
        std::cout<<"size:"<<sizeof(TestSignalOSlot)<<std::endl;
        conn_ = w->sigDestroyed.connect(this, &TestSignalOSlot::onClose);
    }
    ~TestSignalOSlot()
    {
        w_ = nullptr;
    }
    void onClose(TwObject*)
    {
        conn_->expire();
        w_->deleteLater();
        delete this;
    }
    TwWindow* w_;

    std::shared_ptr<TwSignalConnectionBase> conn_;
};
TwTestFun(TestSignal)
{
    TwWindow* ww = new TwWindow;
    ww->setTitle(L"TestSignal");
    ww->setWindowAttribute(Tw::WA_DeleteOnWindowDestroyed, true);
    ww->show();
    ww->deleteLater();

    TestSignalOSlot* ol = new TestSignalOSlot(ww);
    TW_UNUSED(ol);
    return true;
}

TwTestFun(TestScopePtr)
{
    TwWindow* ww = new TwWindow;
    ww->show();
    TwWeakPtr<TwWindow> wwGuard(ww);
    {
        TwScopedPtr<TwWindow> spWindow(ww);
    }

    bool ret = wwGuard.isNull();
    DAssert_M(ret, "ScopedPtr wrong");
    return ret;
    return true;
}

struct WrString
{
    WrString(const std::string& s)
        :str(s)
    {

    }

    WrString(const WrString& s)
        : str(s.str)
    {

    }
    std::string str;
};
 
 struct TestCall
 {
 
     void func1( WrString& msg)
     {

         std::cout<<msg.str<<std::endl;
     }
     void func2(const WrString& msg, int*p)
     {
         std::cout<<msg.str<<std::endl;
         std::cout<<*p<<std::endl;
         delete p;
     }
     void func3(const WrString& msg, int a, int b)
     {
         std::cout<<msg.str<<std::endl;
         std::cout<<a<<std::endl;
         std::cout<<b<<std::endl;
     }
     void func4(const WrString& msg, int a, int b, int c)
     {
         std::cout<<msg.str<<std::endl;
         std::cout<<a<<std::endl;
         std::cout<<b<<std::endl;
         std::cout<<c<<std::endl;
     }
     void func5(const WrString& msg, int a, int b, int c, int d)
     {
         std::cout<<msg.str<<std::endl;
         std::cout<<a<<std::endl;
         std::cout<<b<<std::endl;
         std::cout<<c<<std::endl;
         std::cout<<d<<std::endl;
         delete this;
     }
 };
 
 
 
 TwTestFun(TestAsyncCall)
 {
     TwWindow* ww = new TwWindow;
     ww->setTitle(L"TestAsyncCall");
 
     //Safe Call
     TwAsyncCall(ww, &TwWindow::setVisible, false, TwMessageLoop::currentLoop());
     ww->deleteLater();
 
     bool exist = TwObjectDetect<TestCall>::isTwObjectType;
 //     TwObjectDetect<TestCall> dec;
 //     dec.check<TestCall>(0);
 // 
 //     TwObjectDetect<TwWindow> dec2;
 //     dec2.check<TwWindow>(0);
 
 
     TestCall* cc = new TestCall;
     WrString ss("123xx");
 
     WrString& ress = ss;
 
     int *pi = new int(101);
     int a = 10;
     int b = 20;
     int c = 30;
     int& d = c;
     TwAsyncCall(cc, &TestCall::func1 ,ress , TwMessageLoop::currentLoop());
     TwAsyncCall(cc, &TestCall::func2 ,ss , pi, TwMessageLoop::currentLoop());
     TwAsyncCall(cc, &TestCall::func3 ,ss , a, b,  TwMessageLoop::currentLoop());
     TwAsyncCall(cc, &TestCall::func4 ,ss , a, b, c, TwMessageLoop::currentLoop());
     TwAsyncCall(cc, &TestCall::func5 ,ss , a, b, c, d, TwMessageLoop::currentLoop());
 
     std::cout<<std::boolalpha<<exist;
 //    TwHolderTraits<TestCall>::TwHolder p;
 
     return true;
 }

 TwTestFun(TestRef)
 {
     TwRefPtr<TwRefBase> hah(new TwRefObject<TwRefBase>);

     TwRefPtr<TwRefBase> hah2 = hah;

     bool ret = hah2.refCount() == 2;
     ret;
     DAssert_M(ret, "TestRef");
     return true;
 }

struct  ProperyHolder : public TwPropertyPort0
{
public:
    ProperyHolder(WrString* xx)
        :count_(L"count", 123)
        ,str_(L"name", xx)
        ,age_(L"age", 100)
        ,finds_(L"finds", 10000.0)
    {
        addProperty(&count_);
        addProperty(&str_);
        addProperty(&finds_);
        Xyz_.name_ = L"xyz";
        addProperty(&Xyz_);
    }
    void setCount(int c)
    {
        count_ = c;
    }

    int getCount() const
    {
        return count_;
    }
    TwProperty_0<int> count_;


    uint64 getAge() const
    {
        return age_;
    }

    TwProperty_0<uint64> age_;

    float getFinds() const
    {
        return finds_;
    }

    TwProperty_0<float> finds_;


    void setName(WrString* name)
    {
        str_ = name;
    }

    WrString* getName() const
    {
        return str_;
    }

    TwProperty_0<WrString*> str_;


    WrString* getXyz() const
    {
        return Xyz_;
    }
    TwProperty_0<WrString*> Xyz_;

};

TwTestFun(TestProperty0)
{
    TwScopedPtr<WrString> name(new WrString("overname"));
    TwScopedPtr<WrString> name2(new WrString("overname2"));
     
    ProperyHolder test(name.get());
    TempLog<<test.getCount();
    DAssert(test.getCount()==123);

    test.setCount(999);
    TempLog<<test.getCount();
    DAssert(test.getCount()==999);

    TempLog<<"Age:"<<test.getAge();

    TempLog<<"Finds:"<<test.getFinds();
    test.set(L"finds", L"8.8");
    TempLog<<"Finds:"<<test.getFinds();

    TempLog<<test.getName()->str;
    DAssert(test.getName()->str =="overname");

    test.set(L"name", PropertyHelper<WrString*>::toString(name2.get()));
    TempLog<<test.getName()->str;
    DAssert(test.getName()->str =="overname2");

    test.set(L"xyz", PropertyHelper<WrString*>::toString(name2.get()));
    TempLog<<test.getXyz()->str;

    DAssert(test.getXyz()->str =="overname2");


    return true;
}


struct ObjProperyHolder : public TwDynamicObject
{
    TW_CLASSMEMEBER_BEGIN(ObjProperyHolder, TwDynamicObject);
    TW_PROPERTY(ObjProperyHolder,int,L"xyz", setXyz, getXyz);
    TW_PROPERTY(ObjProperyHolder,std::wstring,L"name", setName, getName);
    TW_CLASSMEMEBER_END 
    ObjProperyHolder()
    {
        xyz_ = 1111;
        name_ = L"haha";
    }

    void setXyz(int xx) 
    {
        xyz_ = xx;
    }
    int getXyz() const 
    {
        return xyz_;
    }
 
    void setName(const std::wstring& name) { name_ = name;}
    std::wstring getName() const { return name_;}

    int xyz_;
    int age_;
    std::wstring name_;
};

struct ObjProperyHolder2 : public ObjProperyHolder
{
    TW_CLASSMEMEBER_BEGIN(ObjProperyHolder2, ObjProperyHolder);
    TW_PROPERTY(ObjProperyHolder2,int,L"xyz2",setXyz, getXyz);
    TW_PROPERTY(ObjProperyHolder2,std::wstring, L"name2", setName, getName);
    TW_CLASSMEMEBER_END

    ObjProperyHolder2()
    {
        xyz_ = 2222;
        name_ = L"haha2";
    }

    void setXyz(int xx) { xyz_ = xx;}
    int getXyz() const {return xyz_;}

    void setName(const std::wstring& name) { name_ = name;}
    std::wstring getName() const { return name_;}

    int xyz_;
    int age_;
    std::wstring name_;
};

TwTestFun(TestProperty)
{
    TwScopedPtr<ObjProperyHolder> holder(new ObjProperyHolder2);

    TwScopedPtr<ObjProperyHolder> holder2(new ObjProperyHolder2);

    TempLog<<holder->getStringProperty(L"name");

    TempLog<<holder->getStringProperty(L"name2");


    //setProperty
    TempLog<<holder->getStringProperty(L"xyz2");
    holder->setStringProperty(L"xyz2", L"98989");
    TempLog<<holder->getStringProperty(L"xyz2");
    DAssert(98989 == (int)holder->getProperty(L"xyz2"));

    //getProperty
    TempLog<<holder->getStringProperty(L"xyz");
    holder->setProperty(L"xyz", 123456);
    TempLog<<holder->getStringProperty(L"xyz");
    int xyz = holder->getProperty(L"xyz");
    TW_UNUSED(xyz);
    DAssert(xyz == 123456);


    TempLog<<holder2->getStringProperty(L"xyz");
    holder2->setProperty(L"xyz", 2014520);
    TempLog<<holder2->getStringProperty(L"xyz");
    int xyzh2 = holder2->getProperty(L"xyz");
    TW_UNUSED(xyzh2);
    DAssert(xyzh2 == 2014520);

    holder2->setProperty(L"name", L"new name");
    DAssert(std::wstring(holder2->getProperty(L"name")) == L"new name");

    //this will hit assert
   // int xyzErr = holder->getPropertyTy<int>(L"xyzErr");

    return true;
}

class TestEvent : public TwEventPort
{
public:
    TestEvent()
    {
        ;
    }
    virtual void onEvent(TwEvent* ev)
    {
        TempLog<<ev->id();

        if (ev->id() == Tw::Ev_Paint)
        {
            deleteLater();
        }
    }

    virtual bool hookHandleEvent(TwEventPort* sender , TwEvent* ev)
    {
        TW_UNUSED(sender);
        DAssert(sender == this);
        ev->accept();
        return false;
    }

};
TwTestFun(TestEventDispatch)
{
    TestEvent* o = new TestEvent;

    TwEvent* newEv = new TwEvent(Tw::Ev_Paint);
    o->installEventHook(o);
    TwEventDispatcher::postEvent(o, newEv);

    return true;
}



//////////////////////////////////////////////////////////////////////////
struct TestVar
{
    TestVar():xx() {;}
    TestVar(int x) : xx(x) {;}
    int xx;
};
void recvint(int a)
{
    TempLog<<a;
}
void recvvoidptr(void* a)
{
    TempLog<<(int)a;
}
void recvchar(char a)
{
    TempLog<<a;
}
void recvVar(TestVar a)
{
    TempLog<<a.xx;
}
void recvCPtr(const TestVar* a)
{
    TempLog<<a->xx;
}
void recvPtr(TestVar* a)
{
    TempLog<<a->xx;
}
void recvstr(const std::wstring& a)
{
    TempLog<<a;
}
TwTestFun(TestVar)
{
    TwVar vint(123);
    recvint(vint);

    void* ptr = (void*)12345678;
    TwVar vvptr = TwVar::fromValue(ptr);
    recvvoidptr(vvptr);

    TestVar tv(9090);
    TwVar vvar = TwVar::fromValue(tv);
    recvVar(vvar);
    tv.xx = 1010;
    recvVar(vvar);

    TestVar tv2(9999);
    TwVar vptr = TwVar::fromValue((const TestVar* const)&tv2);
    recvPtr(vptr);
    tv2.xx = 1010;
    recvCPtr(vptr);

    std::wstring wowo(L"wowo");
    TwVar vstr(wowo);
    recvstr(vstr);

    return true;
}


struct TestMethod : public TwDynamicObject
{
    TW_CLASSMEMEBER_BEGIN(TestMethod, TwDynamicObject)
    TW_CLASSMETHOD(TestMethod, setCount)
    TW_CLASSMETHOD(TestMethod, getCount)
    TW_CLASSMETHOD(TestMethod, setyyc)
    TW_CLASSMETHOD(TestMethod, getyyc)
    TW_CLASSMEMEBER_END
    TestMethod() {;}

    int count_;
    void setCount(int x) { count_ = x;}
    int getCount() const { return count_;}

    int yy_;
    void setyyc(int yy, int count) { yy_ = yy; count_ = count;};
    std::vector<int> getyyc()
    {
        std::vector<int> r; 
        r.push_back(yy_);
        r.push_back(count_);

        return r;
    }
};

struct TestMethod2 : public TestMethod
{
    TW_CLASSMEMEBER_BEGIN(TestMethod2, TestMethod)
        TW_CLASSMETHOD(TestMethod2, setCount2)
        TW_CLASSMETHOD(TestMethod2, getCount2)
        TW_CLASSMETHOD(TestMethod2, setyyc2)
        TW_CLASSMETHOD(TestMethod2, getyyc2)
    TW_CLASSMEMEBER_END

    TestMethod2() {;}

    int count_2;
    void setCount2(int x) 
    {
        count_ = x;
    }
    int getCount2() const
    {
        return count_2;
    }

    int yy_2;
    void setyyc2(int yy, int count)
    { 
        yy_2 = yy; count_2 = count;
    };
    const std::vector<int>& getyyc2()
    {
        r.push_back(yy_2);
        r.push_back(count_2);

        return r;
    }
    std::vector<int> r;

};

TwTestFun(TestInvokeMethod)
{
    TwScopedPtr<TestMethod2> mm(new TestMethod2);

    mm->invokeMethod(L"setCount", 22);
    TwVar c = mm->invokeMethod(L"getCount");
    TempLog<<(int)c;


    mm->invokeMethod(L"setyyc", 90, 101);
    std::vector<int> r = mm->invokeMethod(L"getyyc");
    DAssert(r.size()==2);
    TempLog<<r[0];
    TempLog<<r[1];


    mm->invokeMethod(L"setyyc2", 290, 201);
    r = mm->invokeMethod(L"getyyc2");
    DAssert(r.size()==2);
    TempLog<<r[0];
    TempLog<<r[1];
    
    return true;
}


class TestSignal : public TwObject
{
public:
    TestSignal()
        :sigId(this), sig2(this), sig3(this), sig4(this), sig5(this)
    {
        ;
    }
    TwSignal<void(int)> sigId;
    TwSignal<void(int, std::string)> sig2;
    TwSignal<void(int, float, float)> sig3;
    TwSignal<void(int, float, float, char)> sig4;
    TwSignal<void(int, float, float, char, TwPoint<int> ) > sig5;
};


class ObjectSlot
{
public:
    void operator()(int xx)
    {
        TempLog<<xx;
    }

    void slotFunc(TwObject* sender)
    {
        TW_UNUSED(sender);
        TempLog;
    }
    void slotFunc1(TwObject* sender, int id)
    {
        TW_UNUSED(sender);
        TempLog<<id;
    }

    void slotFunc2(TwObject* sender, int id, std::string name)
    {
        TW_UNUSED(sender);
        TempLog<<id<<name;
    }

    void slotFunc3(TwObject* sender, int id, float x, float y)
    {
        TW_UNUSED(sender);
        TempLog<<id;
        TempLog<<x<<"  "<<y;
    }

    void slotFunc4(TwObject* sender, int id, float x, float y, char cc)
    {
        TW_UNUSED(sender);
        TempLog<<id;
        TempLog<<x<<"  "<<y;
        TempLog<<cc;
    }

    void slotFunc5(TwObject* sender, int id, float x, float y, char cc, TwPoint<int> pt)
    {
        TW_UNUSED(sender);
        TempLog<<id;
        TempLog<<x<<"  "<<y;
        TempLog<<cc;
        TempLog<<"pt:"<<pt.x()<<","<<pt.y();
    }
};

void ffoo(TwObject*, int x)
{
    TempLog<<x;
}

TwTestFun(TestTwSignal)
{
    TestSignal sig;
    ObjectSlot slot;
    std::shared_ptr<TwSignalConnectionBase> s = sig.sigId.connect(&slot, &ObjectSlot::slotFunc1);
    sig.sigId.connect(std::function<void(TwObject*, int)>(ffoo));
    s->expire();

    int id(12);
    sig.sigId.emit(id);

    sig.sig2.connect(&slot,  &ObjectSlot::slotFunc2);
    sig.sig3.connect(&slot,  &ObjectSlot::slotFunc3);
    sig.sig4.connect(&slot,  &ObjectSlot::slotFunc4);
    sig.sig5.connect(&slot,  &ObjectSlot::slotFunc5);

    sig.sig2.emit(100, "haha");
    sig.sig3.emit(100, 0.1f, 0.2f);
    sig.sig4.emit(99, 10.1f, 10.2f, 'y');
    sig.sig5.emit(99, 10.1f, 10.2f, 'y', TwPoint<int>(40,40));

    return true;
}