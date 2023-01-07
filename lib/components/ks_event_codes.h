#pragma once

enum KsEventCode: uint16_t {
    ks_event_empty_queue,
    ks_event_timer_tick,
    ks_event_health_ping,
    ks_event_health_pong,
    ks_event_invalid = 0xFFFF, // TODO: Switch this to some sort of macro (probably provided by asf or something)
};
