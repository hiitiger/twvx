#pragma once

struct TW_BASE_API TwTask
{
    virtual ~TwTask() {;}
    virtual void run() {;}
};
