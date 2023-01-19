#pragma once

#include "ks_component_queued.h"
#include "ks_packet_parser.h"
#include "ks_io.h"

/* Server port, the port the server listens on for incoming connections from the client. */
#define MY_SERVER_PORT  10

namespace kronos {
    class CommandListener : public ComponentQueued {
    public:
        explicit CommandListener(const std::string& name, IoDriver* ioDriver);
        KsCmdResult ProcessEvent(const EventMessage& message) override;
        KsResultType Init() override;

    private:
        IoDriver* m_IoDriver;
        KsResultType Listen();
    };
}