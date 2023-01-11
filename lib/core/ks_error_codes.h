#pragma once

#include "ks_types.h"

#include <cstdint>
#include <climits>

namespace kronos {
    enum KsResult : KsResultType {
        __ks_error_start = INT32_MIN,

        // Apollo Format errors,
        ks_error_apolloformat_status_uninitianalized,
        ks_error_apolloformat_version_uninitianalized,
        ks_error_apolloformat_readwrite_nbytes,
        ks_error_apolloformat_header,
        ks_error_apolloformat_version,

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
        ks_error_file_close,

        // Scheduler related errors
        ks_error_scheduler_rate_exists,
        ks_error_scheduler_rate_missing,

        __ks_error_end,

        ks_success = 0,
    };
}

