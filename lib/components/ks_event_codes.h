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

        // File
        ks_event_file_downlink_begin,
        ks_event_file_downlink_fetch,
        ks_event_file_downlink_continue,
        ks_event_file_downlink_list,

        // Telemetry
        ks_event_tlm_set_active_group,
        ks_event_tlm_list_groups,
        ks_event_tlm_list_channels,

        // Commands
        ks_event_comms_listen,
        ks_event_comms_transmit,
        ks_event_comms_dispatch,

        // Thermal
        ks_event_update_heater,

        // LED
        ks_event_toggle_led,

        // Invalid Event
        ks_event_invalid = UINT16_MAX
    };
}
