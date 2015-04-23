#pragma once

struct TwRefBase
{
    virtual int addRef() = 0;
    virtual int release() = 0;
    virtual int refCount() = 0;
protected:
    virtual ~TwRefBase() { ; }
};