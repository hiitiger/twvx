#pragma once

class TW_BASE_API TwSettings
{
    TW_DISABLE_COPY(TwSettings)
public:
    TwSettings();
     ~TwSettings();

    bool loadFile(const std::wstring& file);
    bool makeNewSetting( const std::wstring& root);
    bool valid() const;
    void save();

    void deleteKey(const std::wstring& path);

    std::wstring readValue(const std::wstring& path);
    bool readValue(const std::wstring& path, std::wstring& value);
    bool writeValue(const std::wstring& path, const std::wstring& value);

    bool readAttribute(const std::wstring& path, const std::wstring& attr, std::wstring& value);
    bool writeAttribute(const std::wstring& path, const std::map<std::wstring, std::wstring>& attr);

    const std::wstring& file() const { return m_file;}

private:
    TwScopedPtr<TwXmlDocument> m_settingDoc;
    std::wstring m_file;
    bool m_dirty;
};