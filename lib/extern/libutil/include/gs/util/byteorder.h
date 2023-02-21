#ifndef GS_UTIL_BYTEORDER_H
#define GS_UTIL_BYTEORDER_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Convert numbers between host and network order.
*/

#include <gs/util/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Convert value from host order to network order
   @param[in] value value to convert.
   @return converted value.
*/
uint16_t util_htons(uint16_t value);

/**
   Convert value from network order to host order
   @param[in] value value to convert.
   @return converted value.
*/
uint16_t util_ntohs(uint16_t value);

/**
   Convert value from host order to network order
   @param[in] value value to convert.
   @return converted value.
*/
uint32_t util_htonl(uint32_t value);

/**
   Convert value from network order to host order
   @param[in] value value to convert.
   @return converted value.
*/
uint32_t util_ntohl(uint32_t value);

/**
   Convert value from host order to network order
   @param[in] value value to convert.
   @return converted value.
*/
uint16_t util_hton16(uint16_t value);

/**
   Convert value from host order to network order
   @param[in] from value to convert.
   @param[out] to value converted.
   @param[in] count element count
*/
void util_hton16_array(const uint16_t * from, uint16_t * to, size_t count);

/**
   Convert value from network order to host order
   @param[in] value value to convert.
   @return converted value.
*/
uint16_t util_ntoh16(uint16_t value);

/**
   Convert value from network order to host order
   @param[in] from value to convert.
   @param[out] to value converted.
   @param[in] count element count
*/
void util_ntoh16_array(const uint16_t * from, uint16_t * to, size_t count);

/**
   Convert value from host order to network order
   @param[in] value value to convert.
   @return converted value.
*/
uint32_t util_hton32(uint32_t value);

/**
   Convert value from host order to network order
   @param[in] from value to convert.
   @param[out] to value converted.
   @param[in] count element count
*/
void util_hton32_array(const uint32_t * from, uint32_t * to, size_t count);

/**
   Convert value from network order to host order
   @param[in] value value to convert.
   @return converted value.
*/
uint32_t util_ntoh32(uint32_t value);

/**
   Convert value from network order to host order
   @param[in] from value to convert.
   @param[out] to value converted.
   @param[in] count element count
*/
void util_ntoh32_array(const uint32_t * from, uint32_t * to, size_t count);

/**
   Convert value from host order to network order
   @param[in] value value to convert.
   @return converted value.
*/
uint64_t util_hton64(uint64_t value);

/**
   Convert value from host order to network order
   @param[in] from value to convert.
   @param[out] to value converted.
   @param[in] count element count
*/
void util_hton64_array(const uint64_t * from, uint64_t * to, size_t count);

/**
   Convert value from network order to host order
   @param[in] value value to convert.
   @return converted value.
*/
uint64_t util_ntoh64(uint64_t value);

/**
   Convert value from network order to host order
   @param[in] from value to convert.
   @param[out] to value converted.
   @param[in] count element count
*/
void util_ntoh64_array(const uint64_t * from, uint64_t * to, size_t count);

/**
   Convert value from host order to network order
   @param[in] value value to convert.
   @return converted value.
*/
float util_htonflt(float value);

/**
   Convert value from host order to network order
   @param[in] from value to convert.
   @param[out] to value converted.
   @param[in] count element count
*/
void util_htonflt_array(const float * from, float * to, size_t count);

/**
   Convert value from network order to host order
   @param[in] value value to convert.
   @return converted value.
*/
float util_ntohflt(float value);

/**
   Convert value from network order to host order
   @param[in] from value to convert.
   @param[out] to value converted.
   @param[in] count element count
*/
void util_ntohflt_array(const float * from, float * to, size_t count);

/**
   Convert value from host order to network order
   @param[in] value value to convert.
   @return converted value.
*/
double util_htondbl(double value);

/**
   Convert value from host order to network order
   @param[in] from value to convert.
   @param[out] to value converted.
   @param[in] count element count
*/
void util_htondbl_array(const double * from, double * to, size_t count);

/**
   Convert value from network order to host order
   @param[in] value value to convert.
   @return converted value.
*/
double util_ntohdbl(double value);

/**
   Convert value from network order to host order
   @param[in] from value to convert.
   @param[out] to value converted.
   @param[in] count element count
*/
void util_ntohdbl_array(const double * from, double * to, size_t count);

/**
   Convert value from host order to big endian.
   @param[in] value value to convert.
   @return converted value.
*/
uint16_t util_htobe16(uint16_t value);

/**
   Convert value from host order to little endian.
   @param[in] value value to convert.
   @return converted value.
*/
uint16_t util_htole16(uint16_t value);

/**
   Convert value from big endian to host order.
   @param[in] value value to convert.
   @return converted value.
*/
uint16_t util_betoh16(uint16_t value);

/**
   Convert value from little endian to host order.
   @param[in] value value to convert.
   @return converted value.
*/
uint16_t util_letoh16(uint16_t value);

/**
   Convert value from host order to big endian.
   @param[in] value value to convert.
   @return converted value.
*/
uint32_t util_htobe32(uint32_t value);

/**
   Convert value from host order to little endian.
   @param[in] value value to convert.
   @return converted value.
*/
uint32_t util_htole32(uint32_t value);

/**
   Convert value from big endian to host order.
   @param[in] value value to convert.
   @return converted value.
*/
uint32_t util_betoh32(uint32_t value);

/**
   Convert value from little endian to host order.
   @param[in] value value to convert.
   @return converted value.
*/
uint32_t util_letoh32(uint32_t value);

/**
   Convert value from host order to big endian.
   @param[in] value value to convert.
   @return converted value.
*/
uint64_t util_htobe64(uint64_t value);

/**
   Convert value from host order to little endian.
   @param[in] value value to convert.
   @return converted value.
*/
uint64_t util_htole64(uint64_t value);

/**
   Convert value from big endian to host order.
   @param[in] value value to convert.
   @return converted value.
*/
uint64_t util_betoh64(uint64_t value);

/**
   Convert value from little endian to host order.
   @param[in] value value to convert.
   @return converted value.
*/
uint64_t util_letoh64(uint64_t value);

/**
   Byte swap.
   @param[in] value value to byteswap.
   @return swapped value
*/
uint16_t gs_bswap_16(uint16_t value);

/**
   Byte swap array.
   @param[in] from from address.
   @param[out] to to address.
   @param[in] count element count.
*/
void gs_bswap_16_array(const uint16_t * from, uint16_t * to, size_t count);

/**
   Byte swap.
   @param[in] value value to byteswap.
   @return swapped value
*/
uint32_t gs_bswap_32(uint32_t value);

/**
   Byte swap array.
   @param[in] from from address.
   @param[out] to to address.
   @param[in] count element count.
*/
void gs_bswap_32_array(const uint32_t * from, uint32_t * to, size_t count);

/**
   Byte swap.
   @param[in] value value to byteswap.
   @return swapped value
*/
uint64_t gs_bswap_64(uint64_t value);

/**
   Byte swap array.
   @param[in] from from address.
   @param[out] to to address.
   @param[in] count element count.
*/
void gs_bswap_64_array(const uint64_t * from, uint64_t * to, size_t count);

/**
   Byte swap.
   @param[in] value value to byteswap.
   @return swapped value
*/
float gs_bswap_float(float value);

/**
   Byte swap.
   @param[in] value value to byteswap.
   @return swapped value
*/
double gs_bswap_double(double value);

/**
   Byte swap array.
   @param[in] from from address.
   @param[out] to to address.
   @param[in] count element count.
*/
void gs_bswap_float_array(const float * from, float * to, size_t count);

#ifdef __cplusplus
}
#endif
#endif
