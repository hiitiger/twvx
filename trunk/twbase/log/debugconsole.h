#pragma once

class DebugConsole
{
public:
    ~DebugConsole(void);

    static DebugConsole* getConsole();

private:
    DebugConsole(void);
private:
};
