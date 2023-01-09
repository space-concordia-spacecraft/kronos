#pragma once
#include "ks_error_codes.h"

namespace kronos {
    enum KsComponentResult: KsResultType {
        ks_component_error = __ks_error_end,
    };
}
