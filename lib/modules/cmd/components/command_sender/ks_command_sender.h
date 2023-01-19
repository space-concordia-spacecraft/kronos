#pragma once

namespace kronos {
    class CommandSender: public ComponentQueued {
    public:
        explicit CommandSender(const std::string& name, IoDriver* ioDriver);
        KsCmdResult ProcessEvent(const EventMessage& message) override;
        KsResultType Init() override;

    private:
        IoDriver* m_IoDriver;
        KsResultType Send(const char* buf, uint8_t bufSize);
    };
}
