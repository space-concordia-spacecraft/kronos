/**
 * \file
 *
 * \brief SAM Two-Wire Interface
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

#include <hpl_i2c_m_async.h>
#include <hpl_twihs_config.h>
#include <utils.h>
#include <utils_assert.h>

static struct _i2c_m_async_device *_twihs0_dev = NULL;

/**
 * \internal Set baudrate for TWIHS
 *
 * \param[in] hw       TWIHS Base Address
 * \param[in] clk      TWIHS peripheral clock rate in HZ
 * \param[in] baudrate TWIHS I2C baudrate in HZ
 */
static int32_t _twihs_set_baudrate(void *const hw, uint32_t clk, uint32_t baudrate);

static void _twihs_init_irq_param(const void *const hw, void *dev);

/**
 * \brief TWIHS I2C Master Async configuration Type
 */
struct _i2cm_async_cfg {
	void *                hw; /*!< instance of TWIHS */
	IRQn_Type             irq;
	hri_twihs_cr_reg_t    ctrl;
	hri_twihs_smbtr_reg_t smbtr;
	hri_twihs_filtr_reg_t filtr;
	hri_twihs_cwgr_reg_t  cwgr;
	uint32_t              clkrate;
};

/**
 * \internal Read one byte from RHR(Receive Holding Register) and prepare next
 *           read
 *
 * \param[in] dev The pointer of I2C Master instance
 * \param[in] msg The pointer of I2C message struct
 *
 * \return The status of the operation
 * \retval ERR_NONE Operation sucessfully
 */
static inline int32_t _i2c_m_async_read(struct _i2c_m_async_device *const dev, struct _i2c_m_msg *msg);

/**
 * \internal Write one byte to THR(Transmit Holding Register) and prepare next
 *           write
 *
 * \param[in] dev The pointer of I2C Master instance
 * \param[in] msg The pointer of I2C message struct
 */
static inline void _i2c_m_async_write(struct _i2c_m_async_device *const dev, struct _i2c_m_msg *msg);

/**
 * \internal Retrieve I2C Master Async configuration instance
 *
 * \param[in] hw The pointer of TWIHS base address
 *
 * \return The I2C Master Async configuration instance
 */
static const struct _i2cm_async_cfg *_get_i2cm_async_cfg(void *hw);

/**
 * \brief Array of I2C Master Async configurations
 */
static const struct _i2cm_async_cfg _i2cm_async_cfgs[1] = {
    {(void *)TWIHS0,
     TWIHS0_IRQn,
     CONF_TWIHS0_CR_REG,
     CONF_TWIHS0_SMBTR_REG,
     CONF_TWIHS0_FILTR_REG,
     CONF_TWIHS0_CWGR_REG,
     CONF_TWIHS0_FREQUENCY / 1000},
};

int32_t _i2c_m_async_init(struct _i2c_m_async_device *const dev, void *const hw)
{
	ASSERT(dev && hw);

	const struct _i2cm_async_cfg *cfg;

	dev->hw = hw;
	cfg     = _get_i2cm_async_cfg(dev->hw);

	// hri_twihs_write_CR_reg(hw, TWIHS_CR_SWRST);
	// hri_twihs_read_RHR_reg(hw);
	hri_twihs_write_CR_reg(dev->hw, cfg->ctrl);
	hri_twihs_write_SMBTR_reg(dev->hw, cfg->smbtr);
	hri_twihs_write_FILTR_reg(dev->hw, cfg->filtr);
	hri_twihs_write_CWGR_reg(dev->hw, cfg->cwgr);

	_twihs_init_irq_param(hw, dev);
	NVIC_ClearPendingIRQ(cfg->irq);
	NVIC_EnableIRQ(cfg->irq);

	return ERR_NONE;
}

int32_t _i2c_m_async_deinit(struct _i2c_m_async_device *const dev)
{
	ASSERT(dev);

	hri_twihs_write_CR_reg(dev->hw, TWIHS_CR_SWRST);
	dev->hw = NULL;

	return ERR_NONE;
}

int32_t _i2c_m_async_enable(struct _i2c_m_async_device *const dev)
{
	ASSERT(dev);

	hri_twihs_write_CR_reg(dev->hw, TWIHS_CR_MSEN);

	return ERR_NONE;
}

int32_t _i2c_m_async_disable(struct _i2c_m_async_device *const dev)
{
	ASSERT(dev);

	hri_twihs_write_CR_reg(dev->hw, TWIHS_CR_MSDIS);

	return ERR_NONE;
}

int32_t _i2c_m_async_set_baudrate(struct _i2c_m_async_device *const dev, uint32_t clkrate, uint32_t baudrate)
{
	ASSERT(dev && baudrate);
	(void)clkrate;

	const struct _i2cm_async_cfg *cfg = _get_i2cm_async_cfg(dev->hw);

	return _twihs_set_baudrate(dev->hw, cfg->clkrate, baudrate);
}

int32_t _i2c_m_async_send_stop(struct _i2c_m_async_device *const dev)
{
	ASSERT(dev && dev->hw);

	hri_twihs_write_CR_reg(dev->hw, TWIHS_CR_STOP);

	return ERR_NONE;
}

void _i2c_m_async_set_irq_state(struct _i2c_m_async_device *const dev, const enum _i2c_m_async_callback_type type,
                                const bool state)
{
	ASSERT(dev);
	(void)dev;
	(void)type;
	(void)state;
	/*
	 * Set IER in _i2c_m_async_transfer and _i2c_m_irq_handler function,
	 * and set IDR in _i2c_m_irq_handler function after a transfer finished.
	 * For avoid SR.TXRDY is set when CR.MSEN is set.
	 */
}

int32_t _i2c_m_async_register_callback(struct _i2c_m_async_device *const dev, enum _i2c_m_async_callback_type type,
                                       FUNC_PTR func)
{
	ASSERT(dev);

	switch (type) {
	case I2C_M_ASYNC_DEVICE_ERROR:
		dev->cb.error = (_i2c_error_cb_t)func;
		break;
	case I2C_M_ASYNC_DEVICE_TX_COMPLETE:
		dev->cb.tx_complete = (_i2c_complete_cb_t)func;
		break;
	case I2C_M_ASYNC_DEVICE_RX_COMPLETE:
		dev->cb.rx_complete = (_i2c_complete_cb_t)func;
		break;
	default:
		/* error */
		break;
	}

	return ERR_NONE;
}

int32_t _i2c_m_async_transfer(struct _i2c_m_async_device *const dev, struct _i2c_m_msg *msg)
{
	ASSERT(dev && msg);

	if (dev->service.msg.flags & I2C_M_BUSY) {
		return I2C_ERR_BUSY;
	}

	msg->flags |= I2C_M_BUSY;
	dev->service.msg = *msg;

	if (msg->flags & I2C_M_RD) {

		if (msg->addr & I2C_M_TEN) {
			hri_twihs_write_MMR_reg(dev->hw,
			                        TWIHS_MMR_DADR(0x78 | (msg->addr >> 8)) | TWIHS_MMR_IADRSZ(1) | TWIHS_MMR_MREAD);
			hri_twihs_write_IADR_reg(dev->hw, msg->addr & 0xff);
		} else {
			hri_twihs_write_MMR_reg(dev->hw, TWIHS_MMR_DADR(msg->addr) | TWIHS_MMR_MREAD);
		}
		hri_twihs_set_IMR_reg(dev->hw, TWIHS_IER_RXRDY);
		/* In single data byte master read, the START and STOP must both be set */
		hri_twihs_write_CR_reg(dev->hw, TWIHS_CR_START | ((msg->len == 1) ? TWIHS_CR_STOP : 0));
	} else {
		if (msg->addr & I2C_M_TEN) {
			hri_twihs_write_MMR_reg(dev->hw, TWIHS_MMR_DADR(0x78 | (msg->addr >> 8)) | TWIHS_MMR_IADRSZ(1));
			hri_twihs_write_IADR_reg(dev->hw, msg->addr & 0xff);
		} else {
			hri_twihs_write_MMR_reg(dev->hw, TWIHS_MMR_DADR(msg->addr));
		}
		hri_twihs_set_IMR_reg(dev->hw, TWIHS_IER_TXRDY | TWIHS_IER_TXCOMP);
	}

	return ERR_NONE;
}

static inline int32_t _i2c_m_async_read(struct _i2c_m_async_device *const dev, struct _i2c_m_msg *msg)
{
	*(msg->buffer) = hri_twihs_read_RHR_reg(dev->hw);
	msg->buffer++;
	msg->len--;

	if (msg->len == 1) {
		/* The STOP bit must be set after the next-to-last data received */
		hri_twihs_write_CR_reg(dev->hw, TWIHS_CR_STOP);
	}

	if (!msg->len) {
		hri_twihs_set_IMR_reg(dev->hw, TWIHS_IER_TXCOMP);
	}

	return ERR_NONE;
}

static inline void _i2c_m_async_write(struct _i2c_m_async_device *const dev, struct _i2c_m_msg *msg)
{
	if (msg->len > 0) {
		hri_twihs_write_THR_reg(dev->hw, *msg->buffer);
		msg->buffer++;
		msg->len--;
	} else {
		hri_twihs_set_IMR_reg(dev->hw, TWIHS_IER_TXCOMP);
		hri_twihs_clear_IMR_reg(dev->hw, TWIHS_IDR_TXRDY);
		hri_twihs_write_CR_reg(dev->hw, TWIHS_CR_STOP);
	}
}

static void _i2c_m_irq_handler(struct _i2c_m_async_device *dev)
{
	uint32_t sr = hri_twihs_read_SR_reg(dev->hw) & hri_twihs_read_IMR_reg(dev->hw);
	ASSERT(dev);
	if ((dev->service.msg.flags & I2C_M_BUSY) == 0) {
		return;
	}
	if (sr & TWIHS_SR_NACK) {
		dev->service.msg.flags &= ~I2C_M_BUSY;
		hri_twihs_clear_IMR_reg(dev->hw,
		                        TWIHS_IDR_TXRDY | TWIHS_IDR_TXCOMP | TWIHS_IDR_RXRDY | TWIHS_IDR_NACK | TWIHS_IDR_OVRE
		                            | TWIHS_IDR_UNRE);
		if (dev->cb.error) {
			dev->cb.error(dev, I2C_NACK);
		}
	} else if (sr & (TWIHS_SR_OVRE | TWIHS_SR_UNRE)) {
		dev->service.msg.flags &= ~I2C_M_BUSY;
		hri_twihs_clear_IMR_reg(dev->hw,
		                        TWIHS_IDR_TXRDY | TWIHS_IDR_TXCOMP | TWIHS_IDR_RXRDY | TWIHS_IDR_NACK | TWIHS_IDR_OVRE
		                            | TWIHS_IDR_UNRE);
		hri_twihs_write_CR_reg(dev->hw, TWIHS_CR_STOP);
		if (dev->cb.error) {
			dev->cb.error(dev, I2C_ERR_ARBLOST);
		}
	} else if (sr & TWIHS_SR_TXRDY) {
		_i2c_m_async_write(dev, &(dev->service.msg));
	} else if (sr & TWIHS_SR_RXRDY) {
		_i2c_m_async_read(dev, &(dev->service.msg));
	} else if ((sr & TWIHS_SR_TXCOMP)) {
		// Read/Write completed when TXCOMP set
		dev->service.msg.flags &= ~I2C_M_BUSY;
		hri_twihs_clear_IMR_reg(dev->hw,
		                        TWIHS_IDR_TXRDY | TWIHS_IDR_TXCOMP | TWIHS_IDR_RXRDY | TWIHS_IDR_NACK | TWIHS_IDR_OVRE
		                            | TWIHS_IDR_UNRE);
		if ((dev->service.msg.flags & I2C_M_RD) && dev->cb.rx_complete) {
			dev->cb.rx_complete(dev);
		} else if (dev->cb.tx_complete) {
			dev->cb.tx_complete(dev);
		}
	}
}

static const struct _i2cm_async_cfg *_get_i2cm_async_cfg(void *hw)
{
	uint8_t i;

	for (i = 0; i < ARRAY_SIZE(_i2cm_async_cfgs); i++) {
		if (_i2cm_async_cfgs[i].hw == hw) {
			return &(_i2cm_async_cfgs[i]);
		}
	}
	return NULL;
}

static int32_t _twihs_set_baudrate(void *const hw, uint32_t clk, uint32_t baudrate)
{
	uint8_t  ckdiv = 0; /* CWGR_CKDIV */
	uint32_t cldiv;     /* CWGR_CLDIV */

	cldiv = clk / (baudrate * 2);

	/* cldiv(CWGR_CLDIV) must fit in 8 bits and
	 * ckdiv(CWGR_CKDIV) must fit in 3 bits
	 *
	 * cldiv may overflow 255 by ckdiv = 0 in previous step,
	 * So here will check cldiv, if cldiv > 255 then will loop ckdiv from 1 to
	 * 7 for find a valid cldiv value
	 */
	while ((cldiv > 255) && (ckdiv < 7)) {
		/* Increase clock divider */
		ckdiv++;
		/* Divide cldiv value */
		cldiv = cldiv >> 1;
	}

	if (cldiv > 255) {
		return ERR_INVALID_DATA;
	}
	/* set CWGR(Clock Waveform Generator Register) */
	hri_twihs_write_CWGR_reg(hw, TWIHS_CWGR_CKDIV(ckdiv) | TWIHS_CWGR_CLDIV(cldiv) | TWIHS_CWGR_CHDIV(cldiv));

	return ERR_NONE;
}

/**
 * \brief Init irq param with the given twihs hardware instance
 */
static void _twihs_init_irq_param(const void *const hw, void *dev)
{
	if (hw == TWIHS0) {
		_twihs0_dev = (struct _i2c_m_async_device *)dev;
	}
}

/**
 * \internal TWI interrupt handler
 */
void TWIHS0_Handler(void)
{
	_i2c_m_irq_handler(_twihs0_dev);
}
