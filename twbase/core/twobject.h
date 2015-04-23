#pragma once
/*
TwObject is the base object type of all framework objects.
It supports:
  * Object tree
  * WeakPointer
  * delete later
*/

class TW_BASE_API TwObject 
{
    TW_NONCOPY(TwObject);
    TW_CLASS_DECLARE(TwObject);
public:
    explicit TwObject(TwObject* parent = nullptr);
    virtual ~TwObject();

    void setParent(TwObject* parent);
    TwObject* parent() const;

    void deleteLater();

private:
    friend class TwObjectGuard;
    TwObjectGuard *getGuard() const;

    void deleteChildren();

private:
    TwObject* m_parent;
    TwObjectGuard *m_guardPtr;
    std::vector<TwObject*> m_children;
};

TW_CLASS_REGISTER(TwObject);


template<class T>
T twObject_cast(TwObject* obj)
{
    if (obj && obj->classInfo()->isDerivedFrom(reinterpret_cast<T>(0)->thisClassInfo()))
    {
        return static_cast<T>(obj);
    }
    return static_cast<T>(nullptr);
}