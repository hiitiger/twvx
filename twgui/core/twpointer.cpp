#include "stable.h"
#include "twpointer.h"



std::multimap<TwObject*,TwObject**> sObjHelper::_ptrmap;


void sObjHelper ::_addObjToPtrmap(TwObject** objaddr)
{
    if (*objaddr)
    {
        (* objaddr)->getHelper()->setHasPtr( true);
        _ptrmap.insert(std::pair<TwObject *,TwObject **>(*objaddr ,objaddr ));
    }
}

void sObjHelper ::_removeObj(TwObject ** objaddr )
{
    if (*objaddr)
    {
        std:: multimap< TwObject*, TwObject**>:: iterator it = sObjHelper::_ptrmap.find(*objaddr );
        std:: multimap< TwObject*, TwObject**>:: iterator end = sObjHelper::_ptrmap.end();
        for (;it != end; )
        {
            if (it->first == * objaddr && it->second == objaddr) //remove的时候只释放一个sPtr的管理权
            {
                sObjHelper::_ptrmap.erase(it++);
                break;
            }
            else{
                ++it;
            }
        }
    }
}

void sObjHelper ::_clearObj(TwObject * obj )
{
    std::multimap<TwObject*,TwObject**>::iterator it = sObjHelper::_ptrmap.find(obj );
    std::multimap<TwObject*,TwObject**>::iterator end = sObjHelper::_ptrmap.end();
    for (; it != end;)
    {
        if (it->first == obj)   //对象被析构时释放所有sPtr的管理权
        {
            *(it->second) = 0;
            sObjHelper::_ptrmap.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}


