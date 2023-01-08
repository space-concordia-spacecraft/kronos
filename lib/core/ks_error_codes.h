#pragma once
#include <cstdint>

namespace kronos {
    enum KsResult:uint16_t {
        ks_success,

        // File related errors
        ks_error_filesystem_sync,
        ks_error_filesystem_init,
        ks_error_filesystem_mount,
        ks_error_filesystem_format,

        ks_error_file_sync,
        ks_error_file_read,
        ks_error_file_write,
        ks_error_file_remove,
        ks_error_file_open,
        ks_error_file_close
    };
}

