#pragma once

enum KsResult: uint16_t {
    ks_success,

    // File related errors
    ks_error_file_sync,
    ks_error_file_read,
    ks_error_file_write,
    ks_error_file_close
};
