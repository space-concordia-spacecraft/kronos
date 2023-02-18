/**
 * \file
 *
 * \brief SAM MCI HPL
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

#ifdef __cplusplus
extern "C" {
#endif

#include <hpl_mci_sync.h>
#include <hpl_mci_async.h>
#include <hpl_hsmci_config.h>
#include <utils_assert.h>

#define HSMCI_SLOT_0_SIZE 4

/** Pointer to hpl device */
static struct _mci_async_device *_mci_dev = NULL;

static void _mci_reset(const void *const hw);
static void _mci_set_speed(const void *const hw, uint32_t speed, uint32_t mck);
static bool _mci_wait_busy(const void *const hw);
static bool _mci_send_cmd_execute(const void *const hw, uint32_t cmdr, uint32_t cmd, uint32_t arg);

/**
 * \brief Reset the HSMCI interface
 *
 * \param hw The pointer to MCI hardware instance
 */
static void _mci_reset(const void *const hw)
{
	uint32_t mr, dtor, sdcr, cstor, cfg;

	ASSERT(hw);

	mr    = hri_hsmci_read_MR_reg(hw);
	dtor  = hri_hsmci_read_DTOR_reg(hw);
	sdcr  = hri_hsmci_read_SDCR_reg(hw);
	cstor = hri_hsmci_read_CSTOR_reg(hw);
	cfg   = hri_hsmci_read_CFG_reg(hw);

	hri_hsmci_write_CR_reg(hw, HSMCI_CR_SWRST);

	hri_hsmci_write_MR_reg(hw, mr);
	hri_hsmci_write_DTOR_reg(hw, dtor);
	hri_hsmci_write_SDCR_reg(hw, sdcr);
	hri_hsmci_write_CSTOR_reg(hw, cstor);
	hri_hsmci_write_CFG_reg(hw, cfg);
	hri_hsmci_write_DMA_reg(hw, 0);
	hri_hsmci_write_CR_reg(hw, HSMCI_CR_PWSEN | HSMCI_CR_MCIEN);
}

/**
 * \brief Set speed of the HSMCI clock.
 *
 * \param hw       The pointer to MCI hardware instance
 * \param speed    HSMCI clock speed in Hz.
 * \param mck      MCK clock speed in Hz.
 */
static void _mci_set_speed(const void *const hw, uint32_t speed, uint32_t mck)
{
	uint32_t clkdiv = 0;
	uint32_t clkodd = 0;
	uint32_t div    = 0;

	/* Speed = MCK clock / (((clkdiv << 1) + clkodd) + 2) */
	if ((speed * 2) < mck) {
		div = (mck / speed) - 2;
		if (mck % speed) {
			/* Ensure that the card speed not be higher than expected. */
			div++;
		}
		clkdiv = div >> 1;
		/* clkodd is the last significant bit of the clock divider (div). */
		clkodd = div % 2;
	} else {
		clkdiv = 0;
		clkodd = 0;
	}
	if (clkodd) {
		hri_hsmci_set_MR_CLKODD_bit(hw);
	} else {
		hri_hsmci_clear_MR_CLKODD_bit(hw);
	}
	hri_hsmci_write_MR_CLKDIV_bf(hw, clkdiv);
}

/**
 * \brief Wait the end of busy signal on data line
 *
 * \param hw       The pointer to MCI hardware instance
 * \return true if success, otherwise false
 */
static bool _mci_wait_busy(const void *const hw)
{
	uint32_t busy_wait = 0xFFFFFFFF;
	uint32_t sr;

	ASSERT(hw);

	do {
		sr = hri_hsmci_read_SR_reg(hw);
		if (busy_wait-- == 0) {
			_mci_reset(hw);
			return false;
		}
	} while (!((sr & HSMCI_SR_NOTBUSY) && ((sr & HSMCI_SR_DTIP) == 0)));
	return true;
}

/**
 * \brief Send a command
 *
 * \param hw         The pointer to MCI hardware instance
 * \param cmdr       CMDR resgister bit to use for this command
 * \param cmd        Command definition
 * \param arg        Argument of the command
 *
 * \return true if success, otherwise false
 */
static bool _mci_send_cmd_execute(const void *const hw, uint32_t cmdr, uint32_t cmd, uint32_t arg)
{
	uint32_t sr;
	ASSERT(hw);

	cmdr |= HSMCI_CMDR_CMDNB(cmd) | HSMCI_CMDR_SPCMD_STD;
	if (cmd & MCI_RESP_PRESENT) {
		cmdr |= HSMCI_CMDR_MAXLAT;
		if (cmd & MCI_RESP_136) {
			cmdr |= HSMCI_CMDR_RSPTYP_136_BIT;
		} else if (cmd & MCI_RESP_BUSY) {
			cmdr |= HSMCI_CMDR_RSPTYP_R1B;
		} else {
			cmdr |= HSMCI_CMDR_RSPTYP_48_BIT;
		}
	}
	if (cmd & MCI_CMD_OPENDRAIN) {
		cmdr |= HSMCI_CMDR_OPDCMD_OPENDRAIN;
	}

	hri_hsmci_write_ARGR_reg(hw, arg);
	hri_hsmci_write_CMDR_reg(hw, cmdr);

	/* Wait end of command */
	do {
		sr = hri_hsmci_read_SR_reg(hw);
		if (cmd & MCI_RESP_CRC) {
			if (sr
			    & (HSMCI_SR_CSTOE | HSMCI_SR_RTOE | HSMCI_SR_RENDE | HSMCI_SR_RCRCE | HSMCI_SR_RDIRE
			       | HSMCI_SR_RINDE)) {
				_mci_reset(hw);
				return false;
			}
		} else {
			if (sr & (HSMCI_SR_CSTOE | HSMCI_SR_RTOE | HSMCI_SR_RENDE | HSMCI_SR_RDIRE | HSMCI_SR_RINDE)) {
				_mci_reset(hw);
				return false;
			}
		}
	} while (!(sr & HSMCI_SR_CMDRDY));

	if (cmd & MCI_RESP_BUSY) {
		if (!_mci_wait_busy(hw)) {
			return false;
		}
	}
	return true;
}

/**
 *  \brief Initialize MCI low level driver.
 */
int32_t _mci_sync_init(struct _mci_sync_device *const mci_dev, void *const hw)
{
	ASSERT(mci_dev && hw);

	mci_dev->hw = hw;

	/* Set the Data Timeout Register to 2 Mega Cycles */
	hri_hsmci_write_DTOR_reg(hw, HSMCI_DTOR_DTOMUL_1048576 | HSMCI_DTOR_DTOCYC(2));

	/* Set Completion Signal Timeout to 2 Mega Cycles */
	hri_hsmci_write_CSTOR_reg(hw, HSMCI_CSTOR_CSTOMUL_1048576 | HSMCI_CSTOR_CSTOCYC(2));

	/* Set Configuration Register */
	hri_hsmci_write_CFG_reg(hw, HSMCI_CFG_FIFOMODE | HSMCI_CFG_FERRCTRL);

	/* Set power saving to maximum value */
	hri_hsmci_write_MR_reg(hw, HSMCI_MR_PWSDIV_Msk);

	/* Enable the HSMCI and the Power Saving */
	hri_hsmci_write_CR_reg(hw, HSMCI_CR_MCIEN | HSMCI_CR_PWSEN);

	return ERR_NONE;
}

/**
 *  \brief Initialize MCI low level driver.
 */
int32_t _mci_async_init(struct _mci_async_device *const mci_dev, void *const hw)
{
	ASSERT(mci_dev && hw);
	struct _mci_async_device *dev = mci_dev;

	/* Do hardware initialize. */
	_mci_sync_init((struct _mci_sync_device *)dev, hw);

	/* Initialize callbacks: must use them */
	dev->cb.read_done_cb  = NULL;
	dev->cb.write_done_cb = NULL;
	dev->cb.error_cb      = NULL;
	_mci_dev              = dev;
	NVIC_DisableIRQ(HSMCI_IRQn);
	NVIC_ClearPendingIRQ(HSMCI_IRQn);
	NVIC_EnableIRQ(HSMCI_IRQn);

	return ERR_NONE;
}
/**
 *  \brief Deinitialize MCI low level driver.
 */
int32_t _mci_sync_deinit(struct _mci_sync_device *const mci_dev)
{
	ASSERT(mci_dev);

	mci_dev->hw = NULL;
	return ERR_NONE;
}
/**
 *  \brief Deinitialize MCI low level driver.
 */
int32_t _mci_async_deinit(struct _mci_async_device *const mci_dev)
{
	ASSERT(mci_dev);
	mci_dev->hw = NULL;
	NVIC_DisableIRQ(HSMCI_IRQn);
	NVIC_ClearPendingIRQ(HSMCI_IRQn);
	return ERR_NONE;
}

/**
 *  \brief Select a device and initialize it
 */
int32_t _mci_sync_select_device(struct _mci_sync_device *const mci_dev, uint8_t slot, uint32_t clock, uint8_t bus_width,
                                bool high_speed)
{
	uint32_t mci_slot      = HSMCI_SDCR_SDCSEL_SLOTA;
	uint32_t mci_bus_width = HSMCI_SDCR_SDCBUS_1;
	void *   hw;

	ASSERT(mci_dev && mci_dev->hw);
	hw = mci_dev->hw;

	if (high_speed) {
		hri_hsmci_set_CFG_HSMODE_bit(hw);
	} else {
		hri_hsmci_clear_CFG_HSMODE_bit(hw);
	}

	_mci_set_speed(hw, clock, CONF_HSMCI_SYS_CLK);

	switch (slot) {
	case 0:
		mci_slot = HSMCI_SDCR_SDCSEL_SLOTA;
		break;
	default:
		return ERR_INVALID_ARG;
	}

	switch (bus_width) {
	case 1:
		mci_bus_width = HSMCI_SDCR_SDCBUS_1;
		break;

	case 4:
		mci_bus_width = HSMCI_SDCR_SDCBUS_4;
		break;

	case 8:
		mci_bus_width = HSMCI_SDCR_SDCBUS_8;
		break;

	default:
		return ERR_INVALID_ARG;
	}
	hri_hsmci_write_SDCR_reg(hw, mci_slot | mci_bus_width);
	return ERR_NONE;
}

/**
 *  \brief Select a device and initialize it
 */
int32_t _mci_async_select_device(struct _mci_async_device *const mci_dev, uint8_t slot, uint32_t clock,
                                 uint8_t bus_width, bool high_speed)
{
	return _mci_sync_select_device((struct _mci_sync_device *)mci_dev, slot, clock, bus_width, high_speed);
}
/**
 *  \brief Deselect a device by an assigned slot
 */
int32_t _mci_sync_deselect_device(struct _mci_sync_device *const mci_dev, uint8_t slot)
{
	/* Nothing to do */
	(void)(mci_dev);
	(void)(slot);

	return ERR_NONE;
}
/**
 *  \brief Deselect a device by an assigned slot
 */
int32_t _mci_async_deselect_device(struct _mci_async_device *const mci_dev, uint8_t slot)
{
	return _mci_sync_deselect_device((struct _mci_sync_device *)mci_dev, slot);
}

/**
 *  \brief Get the maximum bus width of a device
 *         by a selected slot
 */
uint8_t _mci_sync_get_bus_width(struct _mci_sync_device *const mci_dev, uint8_t slot)
{
	(void)(mci_dev);

	switch (slot) {
	case 0:
		return HSMCI_SLOT_0_SIZE;
	default:
		/* Slot number wrong */
		return 0;
	}
}

/**
 *  \brief Get the maximum bus width of a device
 *         by a selected slot
 */
uint8_t _mci_async_get_bus_width(struct _mci_async_device *const mci_dev, uint8_t slot)
{
	return _mci_sync_get_bus_width((struct _mci_sync_device *)mci_dev, slot);
}
/**
 *  \brief Get the high speed capability of the device.
 */
bool _mci_sync_is_high_speed_capable(struct _mci_sync_device *const mci_dev)
{
	(void)(mci_dev);
	return true;
}
/**
 *  \brief Get the high speed capability of the device.
 */
bool _mci_async_is_high_speed_capable(struct _mci_async_device *const mci_dev)
{
	return _mci_sync_is_high_speed_capable((struct _mci_sync_device *)mci_dev);
}

/**
 *  \brief Send 74 clock cycles on the line.
 *   Note: It is required after card plug and before card install.
 */
void _mci_sync_send_clock(struct _mci_sync_device *const mci_dev)
{
	void *hw;
	ASSERT(mci_dev && mci_dev->hw);
	hw = mci_dev->hw;

	/* Configure command */
	hri_hsmci_clear_MR_reg(hw, HSMCI_MR_WRPROOF | HSMCI_MR_RDPROOF | HSMCI_MR_FBYTE);
	hri_hsmci_write_ARGR_reg(hw, 0);
	/* Write and start initialization command */
	hri_hsmci_write_CMDR_reg(hw, HSMCI_CMDR_RSPTYP_NORESP | HSMCI_CMDR_SPCMD_INIT | HSMCI_CMDR_OPDCMD_OPENDRAIN);
	/* Wait end of initialization command */
	while (!hri_hsmci_get_SR_CMDRDY_bit(hw))
		;
}

/**
 *  \brief Send 74 clock cycles on the line.
 *   Note: It is required after card plug and before card install.
 */
void _mci_async_send_init_sequence(struct _mci_async_device *const mci_dev)
{
	_mci_sync_send_clock((struct _mci_sync_device *)mci_dev);
}
/**
 *  \brief Send a command on the selected slot
 */
bool _mci_sync_send_cmd(struct _mci_sync_device *const mci_dev, uint32_t cmd, uint32_t arg)
{
	void *hw;
	ASSERT(mci_dev && mci_dev->hw);
	hw = mci_dev->hw;

	/* Configure command */
	hri_hsmci_clear_MR_reg(hw, HSMCI_MR_WRPROOF | HSMCI_MR_RDPROOF | HSMCI_MR_FBYTE);
	hri_hsmci_write_DMA_reg(hw, 0);
	hri_hsmci_write_BLKR_reg(hw, 0);
	return _mci_send_cmd_execute(hw, 0, cmd, arg);
}

/**
 *  \brief Send a command on the selected slot
 */
bool _mci_async_send_cmd(struct _mci_async_device *const mci_dev, uint32_t cmd, uint32_t arg)
{
	return _mci_sync_send_cmd((struct _mci_sync_device *)mci_dev, cmd, arg);
}
/**
 *  \brief Get 32 bits response of the last command.
 */
uint32_t _mci_sync_get_response(struct _mci_sync_device *const mci_dev)
{
	void *hw;
	ASSERT(mci_dev && mci_dev->hw);
	hw = mci_dev->hw;

	return hri_hsmci_read_RSPR_reg(hw, 0);
}
/**
 *  \brief Get 32 bits response of the last command.
 */
uint32_t _mci_async_get_response(struct _mci_async_device *const mci_dev)
{
	return _mci_sync_get_response((struct _mci_sync_device *)mci_dev);
}

/**
 *  \brief Get 128 bits response of the last command.
 */
void _mci_sync_get_response_128(struct _mci_sync_device *const mci_dev, uint8_t *response)
{
	uint32_t response_32;
	void *   hw;
	ASSERT(mci_dev && mci_dev->hw);
	hw = mci_dev->hw;

	for (uint8_t i = 0; i < 4; i++) {
		response_32 = hri_hsmci_read_RSPR_reg(hw, 0);
		*response   = (response_32 >> 24) & 0xFF;
		response++;
		*response = (response_32 >> 16) & 0xFF;
		response++;
		*response = (response_32 >> 8) & 0xFF;
		response++;
		*response = (response_32 >> 0) & 0xFF;
		response++;
	}
}
/**
 *  \brief Get 128 bits response of the last command.
 */
void _mci_async_get_response_128(struct _mci_async_device *const mci_dev, uint8_t *response)
{
	_mci_sync_get_response_128((struct _mci_sync_device *)mci_dev, response);
}

/**
 *  \brief Send an ADTC command on the selected slot
 *         An ADTC (Addressed Data Transfer Commands)
 *         command is used for read/write access.
 */
bool _mci_sync_adtc_start(struct _mci_sync_device *const mci_dev, uint32_t cmd, uint32_t arg, uint16_t block_size,
                          uint16_t nb_block, bool access_block)
{
	uint32_t cmdr;
	void *   hw;
	ASSERT(mci_dev && mci_dev->hw);
	hw = mci_dev->hw;

	/* No use without dma support */
	(void)access_block;

	hri_hsmci_set_MR_reg(hw, HSMCI_MR_WRPROOF | HSMCI_MR_RDPROOF);
	/* Force byte transfer if needed */
	if (block_size & 0x3) {
		hri_hsmci_set_MR_reg(hw, HSMCI_MR_FBYTE);
	} else {
		hri_hsmci_clear_MR_reg(hw, HSMCI_MR_FBYTE);
	}

	if (cmd & MCI_CMD_WRITE) {
		cmdr = HSMCI_CMDR_TRCMD_START_DATA | HSMCI_CMDR_TRDIR_WRITE;
	} else {
		cmdr = HSMCI_CMDR_TRCMD_START_DATA | HSMCI_CMDR_TRDIR_READ;
	}

	if (cmd & MCI_CMD_SDIO_BYTE) {
		cmdr |= HSMCI_CMDR_TRTYP_BYTE;
		/* Value 0 corresponds to a 512-byte transfer */
		hri_hsmci_write_BLKR_reg(hw, (block_size % 512) << HSMCI_BLKR_BCNT_Pos);
	} else {
		hri_hsmci_write_BLKR_reg(hw, (block_size << HSMCI_BLKR_BLKLEN_Pos) | (nb_block << HSMCI_BLKR_BCNT_Pos));
		if (cmd & MCI_CMD_SDIO_BLOCK) {
			cmdr |= HSMCI_CMDR_TRTYP_BLOCK;
		} else if (cmd & MCI_CMD_STREAM) {
			cmdr |= HSMCI_CMDR_TRTYP_STREAM;
		} else if (cmd & MCI_CMD_SINGLE_BLOCK) {
			cmdr |= HSMCI_CMDR_TRTYP_SINGLE;
		} else if (cmd & MCI_CMD_MULTI_BLOCK) {
			cmdr |= HSMCI_CMDR_TRTYP_MULTIPLE;
		} else {
			return false;
		}
	}
	mci_dev->mci_sync_trans_pos  = 0;
	mci_dev->mci_sync_block_size = block_size;
	mci_dev->mci_sync_nb_block   = nb_block;

	return _mci_send_cmd_execute(hw, cmdr, cmd, arg);
}

/**
 *  \brief Send an ADTC command on the selected slot
 *         An ADTC (Addressed Data Transfer Commands)
 *         command is used for read/write access.
 */
bool _mci_async_adtc_start(struct _mci_async_device *const mci_dev, uint32_t cmd, uint32_t arg, uint16_t block_size,
                           uint16_t nb_block, bool access_block)
{
	uint32_t cmdr;
	void *   hw;
	ASSERT(mci_dev && mci_dev->hw);
	hw = mci_dev->hw;
	hri_hsmci_set_MR_reg(hw, HSMCI_MR_WRPROOF | HSMCI_MR_RDPROOF);
	/* Force byte transfer if needed */
	if (block_size & 0x3) {
		hri_hsmci_set_MR_reg(hw, HSMCI_MR_FBYTE);
	} else {
		hri_hsmci_clear_MR_reg(hw, HSMCI_MR_FBYTE);
	}
	if (cmd & MCI_CMD_WRITE) {
		cmdr = HSMCI_CMDR_TRCMD_START_DATA | HSMCI_CMDR_TRDIR_WRITE;
	} else {
		cmdr = HSMCI_CMDR_TRCMD_START_DATA | HSMCI_CMDR_TRDIR_READ;
	}
	if (cmd & MCI_CMD_SDIO_BYTE) {
		cmdr |= HSMCI_CMDR_TRTYP_BYTE;
		/* Value 0 corresponds to a 512-byte transfer */
		hri_hsmci_write_BLKR_reg(hw, (block_size % 512) << HSMCI_BLKR_BCNT_Pos);
	} else {
		hri_hsmci_write_BLKR_reg(hw, (block_size << HSMCI_BLKR_BLKLEN_Pos) | (nb_block << HSMCI_BLKR_BCNT_Pos));
		if (cmd & MCI_CMD_SDIO_BLOCK) {
			cmdr |= HSMCI_CMDR_TRTYP_BLOCK;
		} else if (cmd & MCI_CMD_STREAM) {
			cmdr |= HSMCI_CMDR_TRTYP_STREAM;
		} else if (cmd & MCI_CMD_SINGLE_BLOCK) {
			cmdr |= HSMCI_CMDR_TRTYP_SINGLE;
		} else if (cmd & MCI_CMD_MULTI_BLOCK) {
			cmdr |= HSMCI_CMDR_TRTYP_MULTIPLE;
		} else {
			return false;
		}
	}
	return _mci_send_cmd_execute(hw, cmdr, cmd, arg);
}
/**
 *  \brief Send a command to stop an ADTC command on the selected slot.
 */
bool _mci_sync_adtc_stop(struct _mci_sync_device *const mci_dev, uint32_t cmd, uint32_t arg)
{
	void *hw;
	ASSERT(mci_dev && mci_dev->hw);
	hw = mci_dev->hw;

	return _mci_send_cmd_execute(hw, HSMCI_CMDR_TRCMD_STOP_DATA, cmd, arg);
}
/**
 *  \brief Send a command to stop an ADTC command on the selected slot.
 */
bool _mci_async_adtc_stop(struct _mci_async_device *const mci_dev, uint32_t cmd, uint32_t arg)
{
	return _mci_sync_adtc_stop((struct _mci_sync_device *)mci_dev, cmd, arg);
}

/**
 *  \brief Read a word on the line.
 */
bool _mci_sync_read_word(struct _mci_sync_device *const mci_dev, uint32_t *value)
{
	uint32_t sr;
	uint8_t  nbytes;
	void *   hw;

	ASSERT(mci_dev && mci_dev->hw);
	hw = mci_dev->hw;

	/* Wait data available */
	nbytes = (mci_dev->mci_sync_block_size & 0x3) ? 1 : 4;
	do {
		sr = hri_hsmci_read_SR_reg(hw);
		if (sr & (HSMCI_SR_UNRE | HSMCI_SR_OVRE | HSMCI_SR_DTOE | HSMCI_SR_DCRCE)) {
			_mci_reset(hw);
			return false;
		}
	} while (!(sr & HSMCI_SR_RXRDY));

	/* Read data */
	*value = hri_hsmci_read_RDR_reg(hw);
	mci_dev->mci_sync_trans_pos += nbytes;

	if (((uint64_t)mci_dev->mci_sync_block_size * mci_dev->mci_sync_nb_block) > mci_dev->mci_sync_trans_pos) {
		return true;
	}

	/* Wait end of transfer */
	do {
		sr = hri_hsmci_read_SR_reg(hw);

		if (sr & (HSMCI_SR_UNRE | HSMCI_SR_OVRE | HSMCI_SR_DTOE | HSMCI_SR_DCRCE)) {
			_mci_reset(hw);
			return false;
		}
	} while (!(sr & HSMCI_SR_XFRDONE));
	return true;
}

/**
 *  \brief Read a word on the line.
 */
bool _mci_async_read_bytes(struct _mci_async_device *const mci_dev, uint32_t *value)
{
	ASSERT(mci_dev && mci_dev->hw);
	/* Read data */
	*value = hri_hsmci_read_RDR_reg(mci_dev->hw);
	return true;
}
/**
 *  \brief Write a word on the line
 */
bool _mci_sync_write_word(struct _mci_sync_device *const mci_dev, uint32_t value)
{
	uint32_t sr;
	uint8_t  nbytes;
	void *   hw;

	ASSERT(mci_dev && mci_dev->hw);
	hw = mci_dev->hw;

	/* Wait data available */
	nbytes = (mci_dev->mci_sync_block_size & 0x3) ? 1 : 4;
	do {
		sr = hri_hsmci_read_SR_reg(hw);
		if (sr & (HSMCI_SR_UNRE | HSMCI_SR_OVRE | HSMCI_SR_DTOE | HSMCI_SR_DCRCE)) {
			_mci_reset(hw);
			return false;
		}
	} while (!(sr & HSMCI_SR_TXRDY));

	/* Write data */
	hri_hsmci_write_TDR_reg(hw, value);
	mci_dev->mci_sync_trans_pos += nbytes;

	if (((uint64_t)mci_dev->mci_sync_block_size * mci_dev->mci_sync_nb_block) > mci_dev->mci_sync_trans_pos) {
		return true;
	}

	/* Wait end of transfer */
	do {
		sr = hri_hsmci_read_SR_reg(hw);

		if (sr & (HSMCI_SR_UNRE | HSMCI_SR_OVRE | HSMCI_SR_DTOE | HSMCI_SR_DCRCE)) {
			_mci_reset(hw);
			return false;
		}
	} while (!(sr & HSMCI_SR_NOTBUSY));
	return true;
}
/**
 *  \brief Write a word on the line
 */
bool _mci_async_write_bytes(struct _mci_async_device *const mci_dev, uint32_t value)
{
	ASSERT(mci_dev && mci_dev->hw);
	/* Write data */
	hri_hsmci_write_TDR_reg(mci_dev->hw, value);
	return true;
}

/**
 *  \brief Start a read blocks transfer on the line
 *  Note: The driver will use the DMA available to speed up the transfer.
 */
bool _mci_sync_start_read_blocks(struct _mci_sync_device *const mci_dev, void *dst, uint16_t nb_block)
{
	uint32_t nb_data;
	uint8_t *ptr = (uint8_t *)dst;
	uint8_t  nbytes;

	ASSERT(mci_dev && mci_dev->hw);
	ASSERT(nb_block);
	ASSERT(dst);

	nb_data = nb_block * mci_dev->mci_sync_block_size;
	nbytes  = (mci_dev->mci_sync_block_size & 0x3) ? 1 : 4;

	while (nb_data) {
		_mci_sync_read_word(mci_dev, (uint32_t *)ptr);
		nb_data -= nbytes;
		ptr += nbytes;
	}

	return true;
}

/**
 *  \brief Start a write blocks transfer on the line
 *  Note: The driver will use the DMA available to speed up the transfer.
 */
bool _mci_sync_start_write_blocks(struct _mci_sync_device *const mci_dev, const void *src, uint16_t nb_block)
{
	uint32_t nb_data;
	uint8_t *ptr = (uint8_t *)src;

	ASSERT(mci_dev && mci_dev->hw);
	ASSERT(nb_block);
	ASSERT(src);

	nb_data = nb_block * mci_dev->mci_sync_block_size;

	if (mci_dev->mci_sync_block_size & 0x3) {
		while (nb_data) {
			_mci_sync_write_word(mci_dev, *ptr);
			nb_data--;
			ptr++;
		}
	} else {
		while (nb_data) {
			_mci_sync_write_word(mci_dev, *(uint32_t *)ptr);
			nb_data -= 4;
			ptr += 4;
		}
	}

	return true;
}

/**
 *  \brief Wait the end of transfer initiated by mci_start_read_blocks()
 */
bool _mci_sync_wait_end_of_read_blocks(struct _mci_sync_device *const mci_dev)
{
	ASSERT(mci_dev && mci_dev->hw);

	/* Always return true for sync read blocks */
	return true;
}

/**
 *  \brief Wait the end of transfer initiated by mci_start_write_blocks()
 */
bool _mci_sync_wait_end_of_write_blocks(struct _mci_sync_device *const mci_dev)
{
	ASSERT(mci_dev && mci_dev->hw);

	/* Always return true for sync write blocks */
	return true;
}

/**
 *  \brief Register the MCI device callback
 */
int32_t _mci_async_register_callback(struct _mci_async_device *mci_dev, const enum _mci_async_callback_type cb_type,
                                     const FUNC_PTR func)
{
	typedef void (*func_t)(void);
	struct _mci_async_device *dev = mci_dev;
	ASSERT(dev && (cb_type <= MCI_ASYNC_ERROR));
	func_t *p_ls  = (func_t *)&dev->cb;
	p_ls[cb_type] = (func_t)func;
	return ERR_NONE;
}
/**
 * \brief Enable/disable mci interrupt
 *
 * param[in] device The pointer to MCI device instance
 * param[in] type The type of interrupt to disable/enable if applicable
 * param[in] state Enable or disable
 */
void _mci_async_set_irq_state(struct _mci_async_device *const device, const enum _mci_async_callback_type type,
                              const bool state)
{
	ASSERT(device);
	if (MCI_ASYNC_READ_DONE == type) {
		if (state) {
			hri_hsmci_set_IMR_RXRDY_bit(device->hw);
		} else {
			hri_hsmci_clear_IMR_RXRDY_bit(device->hw);
		}
	} else if (MCI_ASYNC_WRITE_DONE == type) {
		if (state) {
			hri_hsmci_set_IMR_TXRDY_bit(device->hw);
		} else {
			hri_hsmci_clear_IMR_TXRDY_bit(device->hw);
		}
	} else if (MCI_ASYNC_ERROR == type) {
		if (state) {
			hri_hsmci_set_IMR_reg(device->hw, HSMCI_IMR_UNRE | HSMCI_IMR_OVRE | HSMCI_IMR_DTOE | HSMCI_IMR_DCRCE);
		} else {
			hri_hsmci_clear_IMR_reg(device->hw, HSMCI_IMR_UNRE | HSMCI_IMR_OVRE | HSMCI_IMR_DTOE | HSMCI_IMR_DCRCE);
		}
	}
}
#ifdef __cplusplus
}
#endif
