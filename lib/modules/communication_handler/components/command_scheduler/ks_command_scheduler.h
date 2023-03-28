#pragma once

#include "ks_component_active.h"
#include "ks_command_ids.h"

namespace kronos {

    struct ScheduledCommand {
        uint16_t commandId;
        uint64_t timestamp;

        bool operator<(const ScheduledCommand& other) const {
            return other.timestamp < timestamp;
        }
    };

    class CommandScheduler : public ComponentActive {

    public:
        KsResult ProcessEvent(const EventMessage& message) override;
        [[noreturn]] void Run() override;

    private:
        PriorityQueue <ScheduledCommand> m_ScheduledCommandQueue;

    };

}