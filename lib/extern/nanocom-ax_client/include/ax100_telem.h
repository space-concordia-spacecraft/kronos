#ifndef AX100_TELEM_H_
#define AX100_TELEM_H_
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
 * NanoCom firmware
 *
 */

#include <stdint.h>
#include <param/param_types.h>

/**
 * Define memory space
 */
#define AX100_TELEM_TEMP_BRD		0x00
#define AX100_TELEM_TEMP_PA			0x02
#define AX100_TELEM_LAST_RSSI		0x04
#define AX100_TELEM_LAST_RFERR		0x06
#define AX100_TELEM_TX_COUNTER		0x08
#define AX100_TELEM_RX_COUNTER		0x0C
#define AX100_TELEM_TX_BYTES		0x10
#define AX100_TELEM_RX_BYTES		0x14
#define AX100_TELEM_ACTIVE_CONFIG	0x18
#define AX100_TELEM_BOOT_COUNTER	0x20
#define AX100_TELEM_BOOT_CAUSE		0x24
#define AX100_TELEM_LAST_CONTACT	0x28
#define AX100_TELEM_BGND_RSSI		0x2C
#define AX100_TELEM_TX_DUTY			0x2E
#define AX100_TELEM_TOT_TX_COUNTER	0x30
#define AX100_TELEM_TOT_RX_COUNTER	0x34
#define AX100_TELEM_TOT_TX_BYTES	0x38
#define AX100_TELEM_TOT_RX_BYTES	0x3C

#define AX100_TELEM_SIZE			0x40

/**
 * Setup info about parameters
 */
static const param_table_t ax100_telem[] = {
		{.name = "temp_brd", 		.addr = AX100_TELEM_TEMP_BRD, 				.type = PARAM_INT16, 	.size = sizeof(int16_t)},
		{.name = "temp_pa", 		.addr = AX100_TELEM_TEMP_PA, 				.type = PARAM_INT16, 	.size = sizeof(int16_t)},
		{.name = "last_rssi", 		.addr = AX100_TELEM_LAST_RSSI, 				.type = PARAM_INT16, 	.size = sizeof(int16_t)},
		{.name = "last_rferr", 		.addr = AX100_TELEM_LAST_RFERR, 			.type = PARAM_INT16, 	.size = sizeof(int16_t)},
		{.name = "tx_count", 		.addr = AX100_TELEM_TX_COUNTER, 			.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
		{.name = "rx_count", 		.addr = AX100_TELEM_RX_COUNTER, 			.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
		{.name = "tx_bytes", 		.addr = AX100_TELEM_TX_BYTES, 				.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
		{.name = "rx_bytes", 		.addr = AX100_TELEM_RX_BYTES, 				.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
		{.name = "active_conf",		.addr = AX100_TELEM_ACTIVE_CONFIG,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "boot_count", 		.addr = AX100_TELEM_BOOT_COUNTER, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t),	.flags = PARAM_F_PERSIST},
		{.name = "boot_cause", 		.addr = AX100_TELEM_BOOT_CAUSE, 			.type = PARAM_X32, 		.size = sizeof(uint32_t)},
		{.name = "last_contact", 	.addr = AX100_TELEM_LAST_CONTACT, 			.type = PARAM_UINT32, 	.size = sizeof(uint32_t),	.flags = PARAM_F_PERSIST},
		{.name = "bgnd_rssi",		.addr = AX100_TELEM_BGND_RSSI,				.type = PARAM_INT16, 	.size = sizeof(int16_t)},
		{.name = "tx_duty",			.addr = AX100_TELEM_TX_DUTY,				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "tot_tx_count", 	.addr = AX100_TELEM_TOT_TX_COUNTER, 		.type = PARAM_UINT32, 	.size = sizeof(uint32_t),	.flags = PARAM_F_PERSIST},
		{.name = "tot_rx_count", 	.addr = AX100_TELEM_TOT_RX_COUNTER, 		.type = PARAM_UINT32, 	.size = sizeof(uint32_t),	.flags = PARAM_F_PERSIST},
		{.name = "tot_tx_bytes", 	.addr = AX100_TELEM_TOT_TX_BYTES, 			.type = PARAM_UINT32, 	.size = sizeof(uint32_t),	.flags = PARAM_F_PERSIST},
		{.name = "tot_rx_bytes", 	.addr = AX100_TELEM_TOT_RX_BYTES, 			.type = PARAM_UINT32, 	.size = sizeof(uint32_t),	.flags = PARAM_F_PERSIST},

};

#define AX100_TELEM_COUNT (sizeof(ax100_telem) / sizeof(ax100_telem[0]))

void param_telem_write(uint16_t addr, size_t size, void * item, uint8_t flags);
void * param_telem_read(uint16_t addr, size_t size);
void param_telem_init(void);

#endif /* AX100_TELEM_H_ */
