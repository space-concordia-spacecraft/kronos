#ifndef AX100_PARAM_H_
#define AX100_PARAM_H_
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
 * NanoCom firmware
 *
 */

#include <stdint.h>
#include <param/param_types.h>
#include "ax100.h"

/**
 * Define required memory space,
 * This is used by the HEAP/BSS based memory mappers
 */
#define AX100_CONF_SIZE				0xFF

/**
 * Define memory space
 */
#define AX100_RSSI_OFFSET			0x00
#define AX100_MAX_TEMP				0x02
#define AX100_BGNDRSSI_EMA			0x04

#define AX100_CSP_NODE				0x08
#define AX100_I2C_ENABLE			0x09
#define AX100_CAN_ENABLE			0x0B
#define AX100_EXTPTT_ENABLE			0x0C
#define AX100_LED_ENABLE			0x0D
#define AX100_KISS_USART			0x0E
#define AX100_GOSH_USART			0x0F
#define AX100_I2C_ADDR				0x10
#define AX100_I2C_SPEED_KHZ			0x12
#define AX100_CAN_SPEED_KHZ			0x14

#define AX100_REBOOT_IN				0x18
#define AX100_TX_INHIBIT			0x1C
#define AX100_LOG_STORE_ENABLE		0x20
#define AX100_TX_PWR_LEVEL			0x21

#define AX100_MAX_TX_TIME			0x2C
#define AX100_MAX_IDLE_TIME			0x2E

#define AX100_RTABLE_STR			0x30
#define AX100_RTABLE_STR_SIZE		0x60

#define AX100_LEGACY_HMAC           0x90

#define AX100_KISS_BAUD			0x94

/** Define the memory size */
#define AX100_PARAM_SIZE			0x98

/**
 * Setup info about parameters
 */
static const param_table_t ax100_config[] = {
		{.name = "rssi_offset",		.addr = AX100_RSSI_OFFSET, 			.type = PARAM_INT8, 	.size = sizeof(int8_t)},
		{.name = "max_temp",		.addr = AX100_MAX_TEMP,				.type = PARAM_INT16, 	.size = sizeof(int16_t)},
		{.name = "bgndrssi_ema",	.addr = AX100_BGNDRSSI_EMA,			.type = PARAM_FLOAT, 	.size = sizeof(float)},

		{.name = "csp_node",		.addr = AX100_CSP_NODE,				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "i2c_en",			.addr = AX100_I2C_ENABLE,			.type = PARAM_BOOL, 	.size = sizeof(uint8_t)},
		{.name = "can_en",			.addr = AX100_CAN_ENABLE,			.type = PARAM_BOOL, 	.size = sizeof(uint8_t)},
		{.name = "extptt_en",		.addr = AX100_EXTPTT_ENABLE,		.type = PARAM_BOOL, 	.size = sizeof(uint8_t)},
		{.name = "led_en",			.addr = AX100_LED_ENABLE,			.type = PARAM_BOOL, 	.size = sizeof(uint8_t)},
		{.name = "kiss_usart",		.addr = AX100_KISS_USART,			.type = PARAM_INT8, 	.size = sizeof(uint8_t)},
		{.name = "gosh_usart",		.addr = AX100_GOSH_USART,			.type = PARAM_INT8, 	.size = sizeof(uint8_t)},
		{.name = "i2c_addr",		.addr = AX100_I2C_ADDR,				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "i2c_khz",			.addr = AX100_I2C_SPEED_KHZ,		.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "can_khz",			.addr = AX100_CAN_SPEED_KHZ,		.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},

		{.name = "reboot_in",		.addr = AX100_REBOOT_IN, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "tx_inhibit",		.addr = AX100_TX_INHIBIT, 			.type = PARAM_UINT32, 	.size = sizeof(uint32_t),	.flags = PARAM_F_PERSIST},
		{.name = "log_store",		.addr = AX100_LOG_STORE_ENABLE,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "tx_pwr",			.addr = AX100_TX_PWR_LEVEL,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},

		{.name = "max_tx_time",		.addr = AX100_MAX_TX_TIME,			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "max_idle_time",	.addr = AX100_MAX_IDLE_TIME,		.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},

		{.name = "csp_rtable",		.addr = AX100_RTABLE_STR,			.type = PARAM_STRING,	.size = AX100_RTABLE_STR_SIZE},
		{.name = "legacy_hmac",     .addr = AX100_LEGACY_HMAC,          .type = PARAM_BOOL,     .size = sizeof(uint8_t)},
		{.name = "kiss_baud",     .addr = AX100_KISS_BAUD,          .type = PARAM_UINT32,     .size = sizeof(uint32_t)},
};

static const int ax100_config_count = sizeof(ax100_config) / sizeof(ax100_config[0]);

#endif /* AX100_PARAM_H_ */
