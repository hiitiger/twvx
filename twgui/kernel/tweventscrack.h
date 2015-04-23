#pragma once


//////////////////////////////////////////////////////////////////////////

#define  TW_EVENTACCEPT_CHECK_RETURN(ev) if(ev->accepted()) {return;}


#define  TW_EVENTCRACK_BEGIN(ev) \
         switch (ev->id())\
         {

#define  TW_EVENTCRACK_END() \
         }

#define  TW_EVENTSINK_R_DEF(ev, clstype, r, fun) \
         default: \
         {\
            r->fun(static_cast<clstype*>(ev));\
         }\
         break; 

#define  TW_EVENTSINK_R(ev, clstype, checkId, r, fun) \
        case checkId:\
        {\
            r->fun(static_cast<clstype*>(ev));\
            break;\
        }


#define  TW_EVENTSINK_R_A(ev, clstype, checkId, r, fun) \
        case checkId:\
        {\
            r->fun(static_cast<clstype*>(ev));\
            ev->accept();\
            break;\
        }

#define  TW_EVENTSINK_R_NA(ev, clstype, checkId, r, fun) \
        case checkId:\
        {\
            r->fun(static_cast<clstype*>(ev));\
            ev->reject();\
            break;\
        }

#define  TW_EVENTSINK(ev, clstype, checkId, fun) \
         TW_EVENTSINK_R(ev, clstype, checkId, this, fun)

#define  TW_EVENTSINK_A(ev, clstype, checkId, fun) \
         TW_EVENTSINK_R_A(ev, clstype, checkId, this, fun)