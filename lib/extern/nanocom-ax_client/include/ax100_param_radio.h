#ifndef PARAM_RADIO_H_
#define PARAM_RADIO_H_
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
 * NanoCom firmware
 *
 */

#include <stdint.h>
#include <param/param_types.h>

/* Common parameters for both RX and TX */
#define AX100_FREQ					0x00
#define AX100_BAUD					0x04
#define AX100_MODINDEX				0x08
#define AX100_GUARD					0x0C
#define AX100_PLLRANG_INIT			0x0E
#define AX100_FRM_MODE				0x0F

#define AX100_HMAC_ENABLE			0x10
#define AX100_RS_ENABLE				0x11
#define AX100_CRC32_ENABLE			0x12
#define AX100_RAND_ENABLE			0x13

#define AX100_HMAC_KEY				0x20
#define AX100_HMAC_SIZE				0x10

#define AX100_AX25_CALL				0x30
#define AX100_AX25_CALL_SIZE		0x6

/* TX only parameters */
#define AX100_TX_PREAMBLE			0x40
#define AX100_TX_PREAMBLE_LEN		0x41
#define AX100_TX_PREAMBLE_FLAGS		0x42
#define AX100_TX_INTERFRAME			0x43
#define AX100_TX_INTERFRAME_LEN		0x44
#define AX100_TX_INTERFRAME_FLAGS	0x45
#define AX100_TX_RSSI_BUSY			0x48
#define AX100_TX_KEY_UP_DELAY		0x4A
#define AX100_TX_PA_INPUT_LEVEL		0x4C
#define AX100_TX_BER				0x50
#define AX100_TX_PARAM_SIZE			0x60

/* RX only parameters */
#define AX100_RX_BW					0x40
#define AX100_RX_AFCRANGE			0x44

#define AX100_RX_PARAM_SIZE			0x50

#define COMMON_PARAMS \
	{.name = "freq", 			.addr = AX100_FREQ, 				.type = PARAM_UINT32, 	.size = sizeof(uint32_t)}, \
	{.name = "baud", 			.addr = AX100_BAUD, 				.type = PARAM_UINT32, 	.size = sizeof(uint32_t)}, \
	{.name = "modindex", 		.addr = AX100_MODINDEX, 			.type = PARAM_FLOAT, 	.size = sizeof(float)}, \
	{.name = "guard",			.addr = AX100_GUARD, 				.type = PARAM_UINT16, 	.size = sizeof(uint16_t)}, \
	{.name = "pllrang",			.addr = AX100_PLLRANG_INIT, 		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)}, \
	{.name = "mode",			.addr = AX100_FRM_MODE, 			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)}, \
	{.name = "csp_hmac",		.addr = AX100_HMAC_ENABLE,			.type = PARAM_BOOL, 	.size = sizeof(uint8_t)}, \
	{.name = "csp_rs",			.addr = AX100_RS_ENABLE,			.type = PARAM_BOOL, 	.size = sizeof(uint8_t)}, \
	{.name = "csp_crc",			.addr = AX100_CRC32_ENABLE,			.type = PARAM_BOOL, 	.size = sizeof(uint8_t)}, \
	{.name = "csp_rand",		.addr = AX100_RAND_ENABLE,			.type = PARAM_BOOL, 	.size = sizeof(uint8_t)}, \
	{.name = "csp_hmac_key",	.addr = AX100_HMAC_KEY,				.type = PARAM_DATA,		.size = AX100_HMAC_SIZE}, \
	{.name = "ax25_call",		.addr = AX100_AX25_CALL,			.type = PARAM_STRING,	.size = AX100_AX25_CALL_SIZE}, \

static const param_table_t ax100_tx_config[] = {
	COMMON_PARAMS
	{.name = "preamb",			.addr = AX100_TX_PREAMBLE, 			.type = PARAM_X8, 		.size = sizeof(uint8_t)},
	{.name = "preamblen",		.addr = AX100_TX_PREAMBLE_LEN, 		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "preambflags",		.addr = AX100_TX_PREAMBLE_FLAGS,	.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "intfrm",			.addr = AX100_TX_INTERFRAME, 		.type = PARAM_X8, 		.size = sizeof(uint8_t)},
	{.name = "intfrmlen",		.addr = AX100_TX_INTERFRAME_LEN, 	.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "intfrmflags",		.addr = AX100_TX_INTERFRAME_FLAGS,	.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "rssibusy",		.addr = AX100_TX_RSSI_BUSY,			.type = PARAM_INT16, 	.size = sizeof(int16_t)},
	{.name = "kup_delay",		.addr = AX100_TX_KEY_UP_DELAY,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "pa_level",		.addr = AX100_TX_PA_INPUT_LEVEL,	.type = PARAM_X16, 		.size = sizeof(uint16_t)},
	{.name = "ber",				.addr = AX100_TX_BER,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
};

static const param_table_t ax100_rx_config[] = {
	COMMON_PARAMS
	{.name = "bw", 				.addr = AX100_RX_BW, 				.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
	{.name = "afcrange", 		.addr = AX100_RX_AFCRANGE,			.type = PARAM_INT32, 	.size = sizeof(int32_t)},
};

static const int ax100_config_rx_count = sizeof(ax100_rx_config) / sizeof(ax100_rx_config[0]);
static const int ax100_config_tx_count = sizeof(ax100_tx_config) / sizeof(ax100_tx_config[0]);

#endif /* PARAM_RADIO_H_ */
