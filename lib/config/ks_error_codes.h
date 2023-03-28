#pragma once

namespace kronos {

    enum KsResult : KsResultType {
        // Generic error, avoid using it.
        ks_error = INT32_MIN,
        // Types
        ks_error_queue_create,
        ks_error_queue_push,
        ks_error_queue_pop,
        ks_error_queue_peek,
        ks_error_queue_clear,

        // Busses
        ks_error_bus_exists,
        ks_error_bus_missing,
        ks_error_bus_no_subscribers,
        ks_error_bus_publish,
        ks_error_bus_component_subscribed,

        // Components
        ks_error_component_exists,
        ks_error_component_create,
        ks_error_component_initialize,
        ks_error_component_post_initialize,
        ks_error_component_task_create,
        ks_error_component_task_delete,
        ks_error_component_receive_event,
        ks_error_component_process_event,
        ks_error_component_run,

        // Modules
        ks_error_module_add,
        ks_error_module_exists,
        ks_error_module_initialize,
        ks_error_module_cyclic_dependency,

        // Drivers
        ks_error_driver_missing,

        // Event Message
        ks_error_event_message_missing,

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
        ks_error_file_seek,
        ks_error_file_remove,
        ks_error_file_open,
        ks_error_file_close,
        ks_error_file_size,
        ks_error_file_not_open,
        ks_error_file_max_attempts,

        ks_error_apollo_exporter_open,

        // Scheduler related errors
        ks_error_scheduler_rate_exists,
        ks_error_scheduler_rate_missing,

        // Comms related errors
        ks_error_invalid_packet_header,
        ks_error_invalid_packet,

        // ADD OTHER ERRORS STARTING FROM HERE
        ks_success = 0
    };

}

