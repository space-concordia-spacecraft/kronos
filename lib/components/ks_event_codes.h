#pragma once
#include <cstdint>
#include <climits>

namespace kronos {
    typedef uint16_t KsEventCodeType;

    enum KsEventCode: KsEventCodeType {
        // Event to empty queue for all Queued Components.
        ks_event_empty_queue,

        // Tick event for all Active Components
        ks_event_timer_tick,

        // Health Monitor
        ks_event_health_ping,
        ks_event_health_pong,

        // Invalid Event
        ks_event_invalid = UINT16_MAX, // TODO: Switch this to some sort of macro (probably provided by asf or something)
    };
}
