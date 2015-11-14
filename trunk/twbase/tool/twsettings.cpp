#include "stable.h"

#include "twsettings.h"
#include "xml/twxmldocument.h"


TwSettings::TwSettings()
    : m_dirty(false)
    , m_settingDoc(new TwXmlDocument)
{

}

TwSettings::~TwSettings()
{
    save();
}

bool TwSettings::loadFile(const std::wstring& file)
{
    m_file = file;
    return m_settingDoc->loadFromFile(file);
}

bool TwSettings::makeNewSetting( const std::wstring& root)
{
    m_settingDoc.reset(new TwXmlDocument);
    TwXmlElement entry(root);
    entry.saveToFile(m_file);

    return loadFile(m_file);
}

bool TwSettings::valid() const
{
    return m_settingDoc && m_settingDoc->valid();
}

void TwSettings::save()
{
    if (m_dirty)
    {
        m_settingDoc->rootElement()->saveToFile(m_file);
        m_dirty = false;
    }
}

void TwSettings::deleteKey(const std::wstring& path)
{
    if (m_settingDoc)
    {
        TwXmlElement* elem = m_settingDoc->elementByPath(path);
        if (elem && elem->parentElement())
        {
            elem->parentElement()->deleteChild(elem);
        }
    }
}

std::wstring TwSettings::readValue(const std::wstring& path)
{
    std::wstring value;
    readValue(path, value);
    return value;
}

bool TwSettings::readValue(const std::wstring& path, std::wstring& value)
{
    if (m_settingDoc)
    {
        TwXmlElement* elem = m_settingDoc->elementByPath(path);
        if (elem)
        {
            value = elem->text();
            return true;
        }
    }

    return false;
}

bool TwSettings::writeValue(const std::wstring& path, const std::wstring& value)
{
    if (m_settingDoc)
    {
        TwXmlElement* elem = m_settingDoc->makeElementByPath(path);
        if (elem)
        {
            elem->setText(value);
            m_dirty = true;
            return true;
        }
    }
    return false;
}

bool TwSettings::readAttribute(const std::wstring& path, const std::wstring& attr, std::wstring& value)
{
    if (m_settingDoc)
    {
        TwXmlElement* elem = m_settingDoc->elementByPath(path);
        if (elem && elem->hasAttribute(attr))
        {
            value = elem->attributeValue(attr);
            return true;
        }
    }

    return false;
}

bool TwSettings::writeAttribute(const std::wstring& path, const std::map<std::wstring, std::wstring>& attr)
{
    if (m_settingDoc)
    {
        TwXmlElement* elem = m_settingDoc->makeElementByPath(path);
        if (elem)
        {
            for (auto it = attr.begin(); it != attr.end(); ++it)
            {
                elem->setAttribute(it->first, it->second);
            }
            m_dirty = true;
            return true;
        }
    }

    return false;
}
