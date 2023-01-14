#pragma once

#include "ks_error_codes.h"

namespace kronos {
    // FIXME: Turn this into defines
    enum KsComponentResult : KsResultType {
        __ks_component_error_start = __ks_error_end,

        ks_error_component_healthmonitor_already_registered,

        __ks_component_error_end,
    };
}
