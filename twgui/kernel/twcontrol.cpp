#include "stable.h"
#include "twcontrol.h"

TW_CLASS_IMPLEMENT(TwControl, TwEventPort);

TwControl::TwControl(TwObject* parent)
    : TwEventPort(parent)
{

}

TwControl::~TwControl()
{

}

void TwControl::setName( const std::wstring& name )
{
    m_name = name;
}

const std::wstring& TwControl::name() const
{
    return m_name;
}

