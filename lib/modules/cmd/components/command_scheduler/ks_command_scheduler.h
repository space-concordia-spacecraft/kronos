#pragma once
#include "ks_component_active.h"
#include "ks_default_commands.h"

namespace kronos {

    struct ScheduledCommand {
        KsCommandId commandId;
        uint64_t timestamp;

        bool operator<(const ScheduledCommand& other) const {
            return other.timestamp < timestamp;
        }
    };

    class ComponentCommandScheduled: public ComponentActive {
    public:
        KsCmdResult ProcessEvent(const EventMessage& message) override;
        [[noreturn]] void Run() override;

    private:
        PriorityQueue<ScheduledCommand> m_ScheduledCommandQueue;
    };
}