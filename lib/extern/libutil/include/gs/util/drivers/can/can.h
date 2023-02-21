#ifndef GS_UTIL_DRIVERS_CAN_CAN_H
#define GS_UTIL_DRIVERS_CAN_CAN_H
/* Copyright (c) 2013-2019 GomSpace A/S. All rights reserved. */
/**
   @file

   CAN interface.
*/

#include <gs/util/log.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Default log group for CAN driver.
*/
GS_LOG_GROUP_EXTERN(gs_can_log);
    
/**
   Bit-rate (default).
*/
#define GS_CAN_DEFAULT_BPS    1000000

/**
   Callback for handling received data (from CAN driver).
   @param[in] device hardware device
   @param[in] msg_id standard or extended message id.
   @param[in] extended_msg_id \a true if extended id, \a false if standard id.
   @param[in] data pointer to data.
   @param[in] data_size size of data.
   @param[in] nowMs current relative time in mS.
   @param[in] user_data user data.
   @param[in] cswitch If called from within an ISR (embedded platform), this will none NULL.
*/
typedef void (*gs_can_rxdata_callback_t)(int hdl,
                uint32_t msg_id,
                bool extended_msg_id,
                const void * data,
                size_t data_size,
                uint32_t nowMs,
                void * user_data,
                gs_context_switch_t * cswitch);

/**
   Send CAN message with standard id (11 bits).
   @param[in] device hardware device
   @param[in] msg_id standard CAN message id.
   @param[in] data pointer to data.
   @param[in] data_size size of data.
   @param[in] timeout_ms timeout in mS.
   @return GS_ERROR_FULL if Tx queue is full
   @return_gs_error_t
*/
gs_error_t gs_can_send_standard(uint8_t device, uint32_t msg_id, const void * data, size_t data_size, int timeout_ms);

/**
   Send CAN message with extended id (29 bits).
   @param[in] device hardware device
   @param[in] ext_msg_id extended message id.
   @param[in] data pointer to data.
   @param[in] data_size size of data.
   @param[in] timeout_ms timeout in mS.
   @return GS_ERROR_FULL if Tx queue is full
   @return_gs_error_t
*/
gs_error_t gs_can_send_extended(uint8_t device, uint32_t ext_msg_id, const void * data, size_t data_size, int timeout_ms);

/**
   Set filter and callback for standard message id.
   @param[in] device hardware device
   @param[in] msg_id standard message id.
   @param[in] mask filter mask.
   @param[in] rx_callback callback function.
   @param[in] rx_user_data user data provided in callback.
   @return GS_ERROR_FULL if all message id slots are used.
   @return_gs_error_t
*/
gs_error_t gs_can_set_standard_filter_mask(uint8_t device, uint32_t msg_id, uint32_t mask, gs_can_rxdata_callback_t rx_callback, void * rx_user_data);

/**
   Set filter and callback for extended message id.
   @param[in] device hardware device
   @param[in] ext_msg_id extended message id.
   @param[in] mask filter mask.
   @param[in] rx_callback callback function.
   @param[in] rx_user_data user data provided in callback.
   @return GS_ERROR_FULL if all message id slots are used.
   @return_gs_error_t
*/
gs_error_t gs_can_set_extended_filter_mask(uint8_t device, uint32_t ext_msg_id, uint32_t mask, gs_can_rxdata_callback_t rx_callback, void * rx_user_data);

/**
   Stop CAN layer.
   If a CAN transceiver is present and controlled, it will be disabled.
   @param[in] device hardware device
   @return_gs_error_t
*/
gs_error_t gs_can_stop(uint8_t device);

/**
   Start CAN layer.
   Clear all buffers and start CAN.
   If a CAN transceiver is present and controlled, it will be enabled.
   @param[in] device hardware device
   @return_gs_error_t
*/
gs_error_t gs_can_start(uint8_t device);

/**
   Get current CAN layer error state.
   @param[in] device hardware device
   @param[out] restart_required \a true if CAN layer should be re-started. Pass NULL, if not wanted.
   @return_gs_error_t
*/
gs_error_t gs_can_error_state(uint8_t device, bool * restart_required);

#ifdef __cplusplus
}
#endif
#endif
