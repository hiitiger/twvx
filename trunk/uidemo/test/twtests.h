#pragma once
#include <iostream>

class TwTestsR
{
public:
    class TwTestBase
    {
    public:
        TwTestBase()
        {
            TwTestsR::get()->push(this);
        }
        virtual bool run()
        {
            return true;
        }
    };

    static TwTestsR* get()
    {
        static TwTestsR testR;
        return &testR;
    }
    void push(TwTestBase* t)
    {
        m_tests.push_back(t);
    }

    void run()
    {
//         while (!m_tests.empty())
//         {
//             TwTestBase* t = m_tests.front();
//             m_tests.pop();
//             t->run();
//         }

        for (auto i : m_tests)
        {
            i->run();
        }
    }
    std::vector<TwTestBase*> m_tests;
};


#define  TwTestFunDeclare(funname) \
    static bool D_Test_##funname = true;\
    extern bool Test_##funname();
#define  TwTestFunDeclare_Norun(funname) \
     static bool D_Test_##funname = false;

#define  TwTestFunCall(funname)\
    do \
    {\
        std::cout<<"Test_"#funname<<std::endl;\
        Test_##funname();\
    } while (0);

#define  TwTestFun(funname) \
        bool Test_##funname();\
        class TwTest_##funname : public TwTestsR::TwTestBase \
        {\
            virtual bool run()\
            {\
                if(! (D_Test_##funname))\
                return true;\
                std::cout<<"Test_"#funname<<std::endl;\
                return Test_##funname();\
            }\
        };\
        TwTest_##funname G_test##funname;\
        bool Test_##funname()
