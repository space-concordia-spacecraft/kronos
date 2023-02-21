#ifndef GS_PARAM_PP_PP_H
#define GS_PARAM_PP_PP_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Param Protocol (PP) API - generic interface for getting/setting parameters over SPI, I2C, etc.
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Flags used in gs_pp_xxx() functions.
   @{
*/
/**
   Use checksum in transfer.
   @see gs_pp_checksum8()
*/
#define GS_PP_FLAG_CHECKSUM   0x0001
/**@}*/

/**
   Handle for a protocol connection.
*/
typedef struct gs_pp gs_pp_t;

/**
   Callback for getting a parameter.
*/
typedef gs_error_t (*gs_pp_get_t)(gs_pp_t * pp, uint8_t table_id, uint16_t addr, void * value, size_t value_size, uint32_t flags);

/**
   Callback for setting a parameter.
*/
typedef gs_error_t (*gs_pp_set_t)(gs_pp_t * pp, uint8_t table_id, uint16_t addr, const void * value, size_t value_size, uint32_t flags);

/**
   Callback for setting table lock.
*/
typedef gs_error_t (*gs_pp_get_table_lock_t)(gs_pp_t * pp, uint8_t table_id, bool * value, uint32_t flags);

/**
   Callback for setting table lock.
*/
typedef gs_error_t (*gs_pp_set_table_lock_t)(gs_pp_t * pp, uint8_t table_id, const bool * value, uint32_t flags);

/**
   Handle for a protocol connection.
*/
struct gs_pp {
    /**
       Endian type of slave.
    */
    bool big_endian;
    /**
       Callback function for \a get.
    */
    gs_pp_get_t get;
    /**
       Callback function for \a set.
    */
    gs_pp_set_t set;
    /**
       Callback function for \a get_table_lock.
    */
    gs_pp_get_table_lock_t get_table_lock;
    /**
       Callback function for \a set_table_lock.
    */
    gs_pp_set_table_lock_t set_table_lock;
    /**
       Protocol specifics.
    */
    union {
        /**
           SPI connection.
        */
        struct {
            /**
               SPI slave id.
            */
            uint8_t slave;
        } spi;
        /**
           I2C connection.
        */
        struct {
            /**
               I2C bus.
            */
            uint8_t bus;
            /**
               I2C address.
            */
            uint8_t addr;
        } i2c;
    } pp;
};

/**
   Calculate very simple 8 bit checksum.
   The checksum is calculated by adding all bytes. If the checksum is 0 (zero), the checksum is set to 1 (one).
   @param[in] data data to calculate checksum for.
   @param[in] length data length.
   @return checksum
*/
uint8_t gs_pp_checksum8(const void * data, size_t length);

/**
   Get lock value

   @param[in] pp Handle for connection
   @param[in] table_id Table ID
   @param[out] value Lock state (0 = unlocked, 1 = locked)
   @param[in] flags
   @return_gs_error_t
 */
gs_error_t gs_pp_get_table_lock(gs_pp_t * pp, uint8_t table_id, bool * value, uint32_t flags);

/**
   Set lock value

   @param[in] pp Handle for connection
   @param[in] table_id Table ID
   @param[in] value Lock state (0 = unlocked, 1 = locked)
   @param[in] flags
   @return_gs_error_t
*/
gs_error_t gs_pp_set_table_lock(gs_pp_t * pp, uint8_t table_id, const bool * value, uint32_t flags);

/**
   Get int8.
*/
gs_error_t gs_pp_get_int8(gs_pp_t * pp, uint8_t table_id, uint8_t addr, int8_t * value, size_t count, uint32_t flags);

/**
   Set int8.
*/
gs_error_t gs_pp_set_int8(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const int8_t * value, size_t count, uint32_t flags);

/**
   Get uint8.
*/
gs_error_t gs_pp_get_uint8(gs_pp_t * pp, uint8_t table_id, uint8_t addr, uint8_t * value, size_t count, uint32_t flags);

/**
   Set uint8.
*/
gs_error_t gs_pp_set_uint8(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const uint8_t * value, size_t count, uint32_t flags);

/**
   Get int16.
*/
gs_error_t gs_pp_get_int16(gs_pp_t * pp, uint8_t table_id, uint8_t addr, int16_t * value, size_t count, uint32_t flags);

/**
   Set int16.
*/
gs_error_t gs_pp_set_int16(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const int16_t * value, size_t count, uint32_t flags);

/**
   Get uint16.
*/
gs_error_t gs_pp_get_uint16(gs_pp_t * pp, uint8_t table_id, uint8_t addr, uint16_t * value, size_t count, uint32_t flags);

/**
   Set uint16.
*/
gs_error_t gs_pp_set_uint16(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const uint16_t * value, size_t count, uint32_t flags);

/**
   Get int32.
*/
gs_error_t gs_pp_get_int32(gs_pp_t * pp, uint8_t table_id, uint8_t addr, int32_t * value, size_t count, uint32_t flags);

/**
   Set int32.
*/
gs_error_t gs_pp_set_int32(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const int32_t * value, size_t count, uint32_t flags);

/**
   Get uint32.
*/
gs_error_t gs_pp_get_uint32(gs_pp_t * pp, uint8_t table_id, uint8_t addr, uint32_t * value, size_t count, uint32_t flags);

/**
   Set uint32.
*/
gs_error_t gs_pp_set_uint32(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const uint32_t * value, size_t count, uint32_t flags);

/**
   Get float.
*/
gs_error_t gs_pp_get_float(gs_pp_t * pp, uint8_t table_id, uint8_t addr, float * value, size_t count, uint32_t flags);

/**
   Set float.
*/
gs_error_t gs_pp_set_float(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const float * value, size_t count, uint32_t flags);

/**
   Register commands.
*/
gs_error_t gs_pp_register_commands(void);

#ifdef __cplusplus
}
#endif
#endif
