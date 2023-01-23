#pragma once

#include "ks_component_queued.h"
#include "ks_packet_parser.h"
#include "ks_io.h"

namespace kronos {
    class CommandListener : public ComponentQueued {
    public:
        CommandListener(const String& name, IoDriver* ioDriver);
        void ProcessEvent(const EventMessage& message) override;

    private:
        IoDriver* m_IoDriver;

        KsResultType Listen();
    };
}