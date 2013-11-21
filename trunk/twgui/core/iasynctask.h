#pragma once

struct IAsyncTask
{
    virtual ~IAsyncTask() {;}
    virtual void run()  = 0;
    virtual void finish() {;}
};

