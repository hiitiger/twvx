#pragma once
const int TinyIpcMsg_Zero = 0;

struct TW_BASE_API IpcMsg
{
    int msgId;
    IpcMsg()
    {
        msgId = 0;
    };

    virtual ~IpcMsg(){;}
    virtual void pack(std::string& data) const
    {
        MsgPacker packer(data);
        packer.pushData(msgId);
    }

    virtual void upack(const std::string& data)
    {
        MsgUnpacker unpacker(data);
        unpacker.popData(msgId);;
    }
};