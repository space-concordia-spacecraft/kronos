#pragma once

enum KsResult: uint16_t {
    ks_success,
    ks_error,
    ks_error_missing_component,
    ks_error_missing_bus,
    ks_error_duplicate_component,
    ks_error_duplicate_bus
};
