#pragma once

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
        ks_error_file_not_open,
        ks_error_file_max_attempts,

        // Scheduler related errors
        ks_error_scheduler_rate_exists,
        ks_error_scheduler_rate_missing,

        // Comms related errors
        ks_error_invalid_packet_header,
        ks_error_invalid_packet,

        __ks_error_end,

        ks_success = 0,
    };

}

