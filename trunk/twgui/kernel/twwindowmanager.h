#pragma once

class TwWindow;
class TW_GUI_API TwWindowManager : public TwObject
{
    TW_NONCOPY(TwWindowManager);
public:
    explicit TwWindowManager(TwObject* parent = nullptr);
    virtual ~TwWindowManager();

    void addWindow(TwWindow* window);
    void removeWindow(TwWindow* window);

private:

    std::set<TwWindow*> m_windows;
    bool m_quitPosted;
};