#include "stable.h"
#include "twapp.h"
#include "twwindow.h"

#include "twwindowmanager.h"

TwWindowManager::TwWindowManager( TwObject* parent /*= nullptr*/ )
    : TwObject(parent)
    , m_quitPosted(false)
{
}

TwWindowManager::~TwWindowManager()
{

}

void TwWindowManager::addWindow( TwWindow* window )
{
    m_windows.insert(window);
}

void TwWindowManager::removeWindow( TwWindow* window )
{
    m_windows.erase(window);

    if (m_windows.empty())
    {
        twApp()->quit();
        m_quitPosted = true;
    }
}

