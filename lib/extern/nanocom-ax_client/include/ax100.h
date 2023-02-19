/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
 * NanoCom firmware
 *
 */

#ifndef AX100_H_
#define AX100_H_

#include <stdint.h>

#include <param/param_types.h>
#include <ax100_telem.h>
#include <ax100_param.h>
#include <ax100_param_radio.h>
#include <ax100_param_ax5043.h>

#define AX100_PORT_RPARAM			7
#define AX100_PORT_GNDWDT_RESET		9

/** FRAM MEMORY MAP (From 0x6000 to 0x7FFF is write-protected) */
#define AX100_FRAM_RUNNING			0x5000
#define AX100_FRAM_TELEM			0x5E00
#define AX100_FRAM_GNDWDT			0x5F00
#define AX100_FRAM_LOG				0x3000
#define AX100_FRAM_LOG_SIZE			0x1000

/** FRAM FILENAMES */
#define AX100_FNO_RUNNING			0
#define AX100_FNO_RUNNING_DFL		24
#define AX100_FNO_RX				1
#define AX100_FNO_RX_DFL			25
#define AX100_FNO_TX				5
#define AX100_FNO_TX_DFL			29

/** PARAM INDEX MAP */
#define AX100_PARAM_RUNNING			0
#define AX100_PARAM_RX				1
#define AX100_PARAM_AX5043			3
#define AX100_PARAM_TELEM			4
#define AX100_PARAM_TX(id)			(5 + id)

int ax100_get_hk(param_index_t * mem, uint8_t node, uint32_t timeout);
void cmd_ax100_setup(void);

#endif /* AX100_H_ */
