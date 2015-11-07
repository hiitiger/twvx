#include "stable.h"
#include "IpcLink.h"

IpcLink::IpcLink(const std::string& hostName)
: m_hostName(hostName)
, m_remoteIpcWindow(NULL)
, m_remoteProcess(NULL)
, m_status(Closed)
{

}

IpcLink::~IpcLink(void)
{
    
}

bool IpcLink::sendMessage(const std::string& data)
{
    COPYDATASTRUCT* copydatast = new COPYDATASTRUCT;
    memset(copydatast, 0, sizeof(copydatast));
    std::unique_ptr<COPYDATASTRUCT> copyData(copydatast);
    copyData->dwData = GetCurrentProcessId();
    copyData->cbData = (DWORD)data.length();
    copyData->lpData = (PVOID)data.c_str();
    if (status() == IIpcLink::Connected)
    {
        return (0 != ::SendMessage(m_remoteIpcWindow, WM_COPYDATA, NULL, (LPARAM)copyData.get()));
    }
    else if (status() == IIpcLink::Connecting)
    {
        m_pendingOutQueue.push_back(std::move(copyData));
        return true;
    }
    else
    {
        return false;
    }
}


void IpcLink::addClient(IIpcClient* client)
{
    static int clientId = 0;
    clientId += 1;
    client->saveClientId(this, clientId);
    m_clients[clientId] = client;
}

void IpcLink::removeClient(IIpcClient* client)
{
    for(std::map<int, IIpcClient*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        if (it->second == client)
        {
            m_clients.erase(it);
            break;
        }
    }
}

void IpcLink::onConnecting()
{
    m_status = IIpcLink::Connecting;
}

void IpcLink::onConnect()
{
    m_status = IIpcLink::Connected;

    for (std::map<int, IIpcClient*>::iterator it = m_clients.begin(); it != m_clients.end();)
    {
        auto itNext = it;
        ++itNext;
        it->second->onLinkConnect(this);
        it = itNext;
    }

    for (auto it = m_pendingOutQueue.begin(); it != m_pendingOutQueue.end(); it++)
    {
        ::SendMessage(m_remoteIpcWindow, WM_COPYDATA, NULL, (LPARAM)(*it).get());
    }
    m_pendingOutQueue.clear();
}

void IpcLink::onClosed()
{
    m_status = IIpcLink::Closed;
    for(std::map<int, IIpcClient*>::iterator it = m_clients.begin(); it != m_clients.end();)
    {
        auto itNext = it;
        ++itNext;
        it->second->onLinkClose(this);
        it = itNext;
    }
    m_clients.clear();

    if (m_remoteProcess)
    {
        ::CloseHandle(m_remoteProcess);
    }
    m_remoteProcess = NULL;
    m_remoteIpcWindow = NULL;

    delete this;
}

void IpcLink::onMessage(int clientId, int hostPort, const std::string& message)
{
    if (clientId == 0)
    {
        for(std::map<int, IIpcClient*>::iterator it = m_clients.begin(); it != m_clients.end(); )
        {
            auto itNext = it;
            ++itNext;
            it->second->onMessage(this, hostPort, message);
            it = itNext;
        }
    }
    else
    {
        for(std::map<int, IIpcClient*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
        {
            if (it->first == clientId)
            {
                it->second->onMessage(this, hostPort, message);
                break;
            }
        }
    }
}

