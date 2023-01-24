#pragma once

#include <cstdint>
#include <climits>

namespace kronos {
    typedef uint16_t KsEventCodeType;

    // FIXME: Turn this into defines
    enum KsEventCode : KsEventCodeType {
        // Tick event for scheduled components
        ks_event_scheduler_tick,

        // Logger
        ks_event_log_message,
        ks_event_log_toggle_echo,

        // Health Monitor
        ks_event_health_ping,
        ks_event_health_pong,

        // Parameter DB Save
        ks_event_save_param,

        // Commans
        ks_event_comms_listen,
        ks_event_comms_transmit,
        ks_event_comms_dispatch,

        // LED
        ks_event_toggle_led,

        // Invalid Event
        ks_event_invalid = UINT16_MAX
    };
}
