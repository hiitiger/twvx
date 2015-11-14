#pragma once

class TwXmlElementImpl;
namespace pugi {
    class xml_node;
}
class TW_BASE_API TwXmlElement
{
    TW_DISABLE_COPY(TwXmlElement)
public:
    explicit TwXmlElement(const std::wstring& name);
    ~TwXmlElement();

    TwXmlElement* parentElement();

    std::wstring name();
    std::wstring text();
    void setText(const std::wstring& text);

    int childCount();
    TwXmlElement* childAt(int index);
    TwXmlElement* child(const std::wstring& name);
    TwXmlElement* addChild(const std::wstring& name);
    void addChild(TwXmlElement* child);
    void deleteChild(TwXmlElement* child);

    int attributeCount();
    std::wstring attributeName(int index);
    std::wstring attributeValue(int index);
    std::wstring attributeValue(const std::wstring& attrName);
    bool hasAttribute(const std::wstring& attrName);
    void setAttribute(const std::wstring& attrName, const std::wstring& attrValue);

    void saveToWString(std::wstring& wstr);
    void saveToUtf8(std::string& utf8);
    void saveToFile(const std::wstring& filename);

private:
    void saveToXmlDoc(pugi::xml_node& node, TwXmlElement* elem);

private:
    TwScopedPtr<TwXmlElementImpl> m_dptr;
};


class TW_BASE_API TwXmlDocument
{
    TW_DISABLE_COPY(TwXmlDocument)
public:
    TwXmlDocument();
    ~TwXmlDocument();
     
    bool loadFromUtf8(const std::string& utf8Data);
    bool loadFromFile(const std::wstring& file);
    bool setRootElement(TwXmlElement* elem);

    TwXmlElement* rootElement() const { return m_rootElement.get(); }
    
    TwXmlElement* elementByPath(const std::wstring& path);
    TwXmlElement* makeElementByPath(const std::wstring& path);

    bool valid() const { return rootElement() != nullptr;}

private:
    void makeChildren(pugi::xml_node& elem, TwXmlElement* parent);

private:
    TwScopedPtr<TwXmlElement> m_rootElement;
};