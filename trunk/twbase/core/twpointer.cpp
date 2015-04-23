#include "stable.h"
#include "twpointer.h"
#include "twsync.h"
#include "twsingleton.h"

std::multimap<TwObject*,TwObject**> TwObjectGuard::ptrmap_;
TwDefine_SingleTon(TwLock, _TwSObjMap);

void TwObjectGuard ::addObj(TwObject** objaddr)
{
    if (*objaddr)
    {
        TwScopeLovkV1 lock(_TwSObjMap());
        ptrmap_.insert(std::pair<TwObject *,TwObject **>(*objaddr ,objaddr ));
        (* objaddr)->getGuard()->setHasPtr( true);
    }
}

void TwObjectGuard ::removeObj(TwObject ** objaddr )
{
    if (*objaddr)
    {
		TwScopeLovkV1 lock(_TwSObjMap());
        std:: multimap< TwObject*, TwObject**>:: iterator it = TwObjectGuard::ptrmap_.find(*objaddr );
        for (;it != TwObjectGuard::ptrmap_.end(); )
        {
            if (it->first == * objaddr && it->second == objaddr) //remove just one
            {
                TwObjectGuard::ptrmap_.erase(it++);
                break;
            }
            else
			{
                ++it;
            }
        }
    }
}

void TwObjectGuard ::clearObj(TwObject * obj )
{
	TwScopeLovkV1 lock(_TwSObjMap());
    std::multimap<TwObject*,TwObject**>::iterator it = TwObjectGuard::ptrmap_.find(obj );
    for (; it != TwObjectGuard::ptrmap_.end();)
    {
        if (it->first == obj)   //remove all
        {
            *(it->second) = 0;
            TwObjectGuard::ptrmap_.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}


