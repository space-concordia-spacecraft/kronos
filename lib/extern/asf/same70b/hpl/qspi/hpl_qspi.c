/**
 * \file
 *
 * \brief QSPI Driver
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include <hpl_qspi_sync.h>
#include <utils_assert.h>
#include "hpl_qspi_config.h"

/**
 * \brief Memory copy function.
 *
 * \param dst  Pointer to destination buffer.
 * \param src  Pointer to source buffer.
 * \param count  Bytes to be copied.
 */
static void _qspi_memcpy(uint8_t *dst, uint8_t *src, uint32_t count)
{
	while (count--) {
		*dst++ = *src++;
	}
}

/**
 * \brief Ends ongoing transfer by releasing CS of QSPI peripheral.
 *
 * \param qspi  Pointer to an Qspi instance.
 */
static inline void _qspi_end_transfer(struct _qspi_sync_dev *dev)
{
	ASSERT(dev);
	hri_qspi_write_CR_reg(dev->prvt, QSPI_CR_LASTXFER);
}

int32_t _qspi_sync_init(struct _qspi_sync_dev *dev, void *const hw)
{
	ASSERT(dev && hw);
	dev->prvt = hw;
	hri_qspi_write_CR_reg(dev->prvt, QSPI_CR_SWRST);

	hri_qspi_write_MR_reg(hw,
	                      QSPI_MR_SMM_MEMORY | QSPI_MR_LLB_DISABLED | QSPI_MR_CSMODE_LASTXFER | QSPI_MR_NBBITS_8_BIT
	                          | QSPI_MR_DLYBCT(0) | QSPI_MR_DLYCS(CONF_QSPI_DLYCS));

	hri_qspi_write_SCR_reg(hw,
	                       CONF_QSPI_CPOL << QSPI_SCR_CPOL_Pos | CONF_QSPI_CPHA << QSPI_SCR_CPHA_Pos
	                           | QSPI_SCR_SCBR(CONF_QSPI_BAUD_RATE) | QSPI_SCR_DLYBS(CONF_QSPI_DLYBS));
	return ERR_NONE;
}

int32_t _qspi_sync_deinit(struct _qspi_sync_dev *dev)
{
	hri_qspi_write_CR_reg(dev->prvt, QSPI_CR_SWRST | QSPI_CR_QSPIDIS);

	return ERR_NONE;
}

int32_t _qspi_sync_enable(struct _qspi_sync_dev *dev)
{
	hri_qspi_write_CR_reg(dev->prvt, QSPI_CR_QSPIEN);
	return ERR_NONE;
}

int32_t _qspi_sync_disable(struct _qspi_sync_dev *dev)
{
	hri_qspi_write_CR_reg(dev->prvt, QSPI_CR_QSPIDIS);
	return ERR_NONE;
}

static void _qspi_sync_command_set_ifr(struct _qspi_sync_dev *dev, const struct _qspi_command *cmd)
{
	void *hw = dev->prvt;

	if (cmd->inst_frame.bits.addr_en) {
		hri_qspi_write_IAR_reg(hw, cmd->address);
	}

	if (cmd->inst_frame.bits.inst_en) {
		hri_qspi_write_ICR_INST_bf(hw, cmd->instruction);
	}

	if (cmd->inst_frame.bits.opt_en) {
		hri_qspi_write_ICR_OPT_bf(hw, cmd->option);
	}

	hri_qspi_write_IFR_reg(hw, cmd->inst_frame.word);
}

static void _qspi_sync_run_transfer(struct _qspi_sync_dev *dev, const struct _qspi_command *cmd)
{
	void *   hw       = dev->prvt;
	uint8_t *qspi_mem = (uint8_t *)QSPIMEM_ADDR;

	if (cmd->inst_frame.bits.addr_en) {
		qspi_mem += cmd->address;
	}

	/* To synchronize system bus accesses */
	hri_qspi_read_IFR_reg(hw);

	ASSERT(cmd->tx_buf || cmd->rx_buf);

	if (cmd->tx_buf) {
		_qspi_memcpy((uint8_t *)qspi_mem, (uint8_t *)cmd->tx_buf, cmd->buf_len);
	} else {
		_qspi_memcpy((uint8_t *)cmd->rx_buf, (uint8_t *)qspi_mem, cmd->buf_len);
	}

	__DSB();
	__ISB();
	_qspi_end_transfer(dev);
}

int32_t _qspi_sync_serial_run_command(struct _qspi_sync_dev *dev, const struct _qspi_command *cmd)
{
	_qspi_sync_command_set_ifr(dev, cmd);

	if (cmd->inst_frame.bits.data_en) {
		_qspi_sync_run_transfer(dev, cmd);
	}

	while (!hri_qspi_get_SR_INSTRE_bit(dev->prvt)) {
		;
	}
	return ERR_NONE;
}
