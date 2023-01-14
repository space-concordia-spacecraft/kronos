#pragma once

#include <cstdint>
#include <climits>

namespace kronos {
    typedef uint16_t KsEventCodeType;

    // FIXME: Turn this into defines
    enum KsEventCode : KsEventCodeType {
        // Event to empty queue for all Queued Components.
        ks_event_empty_queue,

        // Tick event for scheduled components
        ks_event_scheduler_tick,

        // Health Monitor
        ks_event_health_ping,
        ks_event_health_pong,

        // Parameter DB Save
        ks_event_save_param,

        // Command
        ks_event_command_dispatch,

        // Invalid Event
        ks_event_invalid = UINT16_MAX
    };
}
