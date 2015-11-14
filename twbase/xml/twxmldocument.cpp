#include "stable.h"
#include "pugixml.hpp"
#include "twxmldocument.h"

struct Attr
{
    std::wstring name;
    std::wstring value;
};

class TwXmlElementImpl
{
public:
    TwXmlElement* parent;
    std::wstring name;
    std::wstring text;
    std::vector<Attr> attributes;
    std::vector<TwXmlElement*> children;
};

TwXmlElement::TwXmlElement(const std::wstring& name)
    : m_dptr(new TwXmlElementImpl())
{
    m_dptr->parent = nullptr;
    m_dptr->name = name;
}

TwXmlElement::~TwXmlElement()
{
    for (auto it = m_dptr->children.begin(); it != m_dptr->children.end(); ++it)
    {
        delete *it;
    }

    m_dptr->children.clear();
}

TwXmlElement* TwXmlElement::parentElement()
{
    return m_dptr->parent;
}

std::wstring TwXmlElement::name()
{
    return m_dptr->name;
}

std::wstring TwXmlElement::text()
{
    return m_dptr->text;
}

void TwXmlElement::setText(const std::wstring& text)
{
    m_dptr->text = text;
}

int TwXmlElement::childCount()
{
    return static_cast<int>(m_dptr->children.size());
}

TwXmlElement* TwXmlElement::childAt(int index)
{
    if (index < 0 || index >= childCount())
    {
        return NULL;
    }
    return m_dptr->children.at(index);
}

TwXmlElement* TwXmlElement::child(const std::wstring& name)
{
    auto it = std::find_if(m_dptr->children.begin(), m_dptr->children.end(), [&](TwXmlElement* elem) {return elem->name() == name;});
    if (it != m_dptr->children.end())
    {
        return (*it);
    }
    else
    {
        return nullptr;
    }
}

TwXmlElement* TwXmlElement::addChild(const std::wstring& name)
{
    TwXmlElement* child = new TwXmlElement(name);
    addChild(child);
    return child;
}

void TwXmlElement::addChild(TwXmlElement* child)
{
    child->m_dptr->parent = this;

    m_dptr->children.push_back(child);
}

void TwXmlElement::deleteChild(TwXmlElement* child)
{
    auto it = std::find(m_dptr->children.begin(), m_dptr->children.end(), child);
    if (it != m_dptr->children.end())
    {
        delete child;
        m_dptr->children.erase(it);
    }
}

int TwXmlElement::attributeCount()
{
    return static_cast<int>(m_dptr->attributes.size());
}

bool TwXmlElement::hasAttribute(const std::wstring& attrName)
{
    return std::find_if(m_dptr->attributes.begin(), m_dptr->attributes.end(), [&](const Attr& attr) {return attr.name == attrName;}) != m_dptr->attributes.end();
}

std::wstring TwXmlElement::attributeName(int index)
{
    if (index < 0 || index >= attributeCount())
    {
        return L"";
    }
    return m_dptr->attributes.at(index).name;
}

std::wstring TwXmlElement::attributeValue(int index)
{
    if (index < 0 || index >= attributeCount())
    {
        return L"";
    }
    return m_dptr->attributes.at(index).value;
}


std::wstring TwXmlElement::attributeValue(const std::wstring& attrName)
{
    for (auto it = m_dptr->attributes.begin(); it != m_dptr->attributes.end(); ++it)
    {
        if (it->name == attrName)
        {
            return it->value;
        }
    }
    return L"";
}

void TwXmlElement::setAttribute(const std::wstring& attrName, const std::wstring& attrValue)
{
    auto it = std::find_if(m_dptr->attributes.begin(), m_dptr->attributes.end(), [&](const Attr& attr) {return attr.name == attrName;});
    if (it != m_dptr->attributes.end())
    {
        it->value = attrValue;
    }
    else
    {
        Attr attr;
        attr.name = attrName;
        attr.value = attrValue;
        m_dptr->attributes.push_back(attr);
    }
}

class xmlwriter_wchar : public pugi::xml_writer
{
public:
    std::wstring& m_output;
    xmlwriter_wchar(std::wstring& output)
        : m_output(output)
    {

    }
    virtual void write(const void* data, size_t size)
    {
        m_output.append((const wchar_t*)(data), size);
    }
};

class xmlwriter_uft8 : public pugi::xml_writer
{
public:
    std::string& m_output;
    xmlwriter_uft8(std::string& output)
        : m_output(output)
    {

    }
    virtual void write(const void* data, size_t size)
    {
        m_output.append((const char*)(data), size);
    }
};

class xmlwriter_uft8_file : public pugi::xml_writer
{
public:
    TwFile& m_file;
    xmlwriter_uft8_file(TwFile& file)
        : m_file(file)
    {

    }
    virtual void write(const void* data, size_t size)
    {
        m_file.write((const char*)(data), size);
    }
};


void TwXmlElement::saveToXmlDoc(pugi::xml_node& parent, TwXmlElement* elem)
{
    pugi::xml_node root = parent.append_child(elem->name().c_str());
    pugi::xml_node textnode = root.append_child(pugi::node_pcdata);
    textnode.set_value(elem->text().c_str());
    for (auto it = elem->m_dptr->attributes.begin(); it != elem->m_dptr->attributes.end(); ++it)
    {
        root.append_attribute(it->name.c_str()).set_value(it->value.c_str());
    }

    for (auto it = elem->m_dptr->children.begin(); it != elem->m_dptr->children.end(); ++it)
    {
        saveToXmlDoc(root, *it);
    }

}

void TwXmlElement::saveToWString(std::wstring& wstr)
{
    pugi::xml_document xmlDoc;
    saveToXmlDoc(xmlDoc, this);
    xmlDoc.save(xmlwriter_wchar(wstr), L"\t", pugi::format_default, pugi::encoding_wchar);
}

void TwXmlElement::saveToUtf8(std::string& utf8)
{
    pugi::xml_document xmlDoc;
    saveToXmlDoc(xmlDoc, this);
    xmlDoc.save(xmlwriter_uft8(utf8), L"\t", pugi::format_default, pugi::encoding_utf8);
}

void TwXmlElement::saveToFile(const std::wstring& filename)
{
    pugi::xml_document xmlDoc;
    saveToXmlDoc(xmlDoc, this);
    TwFile file(filename);
    file.open(TwFile::ReadWrite | TwFile::Truncate);
    xmlDoc.save(xmlwriter_uft8_file(file), L"\t", pugi::format_default, pugi::encoding_utf8);
}

//////////////////////////////////////////////////////////////////////////
TwXmlDocument::TwXmlDocument()
    : m_rootElement(nullptr)
{

}

TwXmlDocument::~TwXmlDocument()
{

}

bool TwXmlDocument::loadFromUtf8(const std::string& utf8Data)
{
    pugi::xml_document xmlDoc;
    std::string data = utf8Data;
    pugi::xml_parse_result result = xmlDoc.load_buffer_inplace((void*)data.c_str(), data.size(), pugi::parse_default, pugi::encoding_utf8);
    bool ok = result.status == pugi::status_ok;
    if (ok)
    {
        pugi::xml_node root = xmlDoc.first_child();
        m_rootElement.reset(new TwXmlElement(root.name()));
        m_rootElement->setText(root.text().get());

        pugi::xml_attribute attr = root.first_attribute();
        while (attr)
        {
            m_rootElement->setAttribute(attr.name(), attr.value());
            attr = attr.next_attribute();
        }

        makeChildren(root, m_rootElement.get());
    }

    return ok;
}

bool TwXmlDocument::loadFromFile(const std::wstring& filename)
{
    TwFile file(filename);
    if (file.open(TwFile::ReadOnly))
    {
        std::string data;
        data.resize((size_t)file.size());
        file.read(const_cast<char*>(data.c_str()), data.size());

        pugi::xml_document xmlDoc;
        pugi::xml_parse_result result = xmlDoc.load_buffer_inplace((void*)data.c_str(), data.size(), pugi::parse_default, pugi::encoding_auto);
        bool ok = result.status == pugi::status_ok;
        if (ok)
        {
            pugi::xml_node root = xmlDoc.first_child();
            m_rootElement.reset(new TwXmlElement(root.name()));
            m_rootElement->setText(root.text().get());

            pugi::xml_attribute attr = root.first_attribute();
            while (attr)
            {
                m_rootElement->setAttribute(attr.name(), attr.value());
                attr = attr.next_attribute();
            }

            makeChildren(root, m_rootElement.get());
        }

        return ok;
    }
    else
    {
        return false;
    }
}

bool TwXmlDocument::setRootElement(TwXmlElement* elem)
{
    m_rootElement.reset(new TwXmlElement(elem->name()));
    return true;
}

TwXmlElement* TwXmlDocument::elementByPath(const std::wstring& path)
{
    std::vector<std::wstring> keyPath = TwUtils::splitString(path, L'/');
    keyPath.erase(std::remove(keyPath.begin(), keyPath.end(), L""), keyPath.end());

    TwXmlElement* elem = m_rootElement.get();
    if (!elem || keyPath.size() == 0 || elem->name() != keyPath[0])
    {
        return nullptr;
    }

    for (int i = 1; i != keyPath.size() ; ++i)
    {
        if (elem != nullptr)
        {
            elem = elem->child(keyPath[i]);
        }
        else
        {
            break;
        }
    }

    return elem;

}

TwXmlElement* TwXmlDocument::makeElementByPath(const std::wstring& path)
{
    std::vector<std::wstring> keyPath = TwUtils::splitString(path, L'/');
    keyPath.erase(std::remove(keyPath.begin(), keyPath.end(), L""), keyPath.end());
    
    TwXmlElement* elem = m_rootElement.get();
    if (!elem || keyPath.size() == 0 || elem->name() != keyPath[0])
    {
        return nullptr;
    }

    int i = 1;
    for (; i != keyPath.size() ; ++i)
    {
        if (!keyPath[i].empty())
        {
            if (elem->child(keyPath[i]))
            {
                elem = elem->child(keyPath[i]);
            }
            else
            {
                break;
            }
        }
    }
    if ( i == keyPath.size())
    {
        return elem;
    }
    else
    {
        for (; i != keyPath.size(); ++i)
        {
            elem = elem->addChild(keyPath[i]);
        }
        return elem;
    }
}

void TwXmlDocument::makeChildren(pugi::xml_node& elem, TwXmlElement* parent)
{
    pugi::xml_node childElem = elem.first_child();
    while (childElem)
    {
        if (childElem.type() == pugi::node_element)
        {
            TwXmlElement* child = new TwXmlElement(childElem.name());

            child->setText(childElem.text().get());

            pugi::xml_attribute attr = childElem.first_attribute();
            while (attr)
            {
                child->setAttribute(attr.name(), attr.value());
                attr = attr.next_attribute();
            }

            parent->addChild(child);
            makeChildren(childElem, child);
        }

        childElem = childElem.next_sibling();
    }
}
