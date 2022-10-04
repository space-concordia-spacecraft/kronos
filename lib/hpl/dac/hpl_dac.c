/**
 * \file
 *
 * \brief SAM Digital to Analog Converter
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

#include <compiler.h>
#include <hpl_dac_async.h>
#include <hpl_dac_config.h>
#include <hpl_dac_sync.h>
#include <utils_assert.h>

#define DACC_MR_MAXS0_Pos 0
#define DACC_MR_MAXS1_Pos 1
#define DACC_MR_DIFF_Pos 23
#define DACC_TRIGR_TRGEN0_Pos 0
#define DACC_TRIGR_TRGEN1_Pos 1

/** \conf INTERNAL */
static inline void _dac_write_protection_disable(void *const hw);
static int32_t     _dac_init(void *const hw);
static inline void _dac_deinit(void *const hw);
static inline void _dac_enable_channel(void *const hw, const uint8_t ch);
static inline void _dac_disable_channel(void *const hw, const uint8_t ch);
static inline bool _dac_is_channel_enabled(void *const hw, const uint8_t ch);
/** \endcond */

/**
 * \brief DAC configuration type
 */
struct dac_configuration {
	hri_dacc_mr_reg_t    mr;
	hri_dacc_trigr_reg_t trigr;
};

/**
 * \brief Array of DAC configurations
 */
static struct dac_configuration _dac = {
    (DACC_MR_PRESCALER(CONF_DAC_CLOCK_PRESCALER)) | (CONF_DAC_DIFF << DACC_MR_DIFF_Pos)
        | ((CONF_DAC_CHANNEL1_MODE & 0x2) << DACC_MR_MAXS1_Pos) | ((CONF_DAC_CHANNEL0_MODE & 0x2) << DACC_MR_MAXS0_Pos),
    (DACC_TRIGR_OSR1(CONF_DAC_OSR1)) | (DACC_TRIGR_OSR0(CONF_DAC_OSR0)) | (DACC_TRIGR_TRGSEL1(CONF_DAC_TRGSEL1))
        | (DACC_TRIGR_TRGSEL0(CONF_DAC_TRGSEL0)) | ((CONF_DAC_CHANNEL1_MODE & 0x1) << DACC_TRIGR_TRGEN1_Pos)
        | ((CONF_DAC_CHANNEL0_MODE & 0x1) << DACC_TRIGR_TRGEN0_Pos)};

/** Pointer to hpl device */
static struct _dac_async_device *_dac_dev = NULL;

/**
 * \brief Initialize synchronous DAC
 */
int32_t _dac_sync_init(struct _dac_sync_device *const device, void *const hw)
{
	ASSERT(device);

	device->hw = hw;

	return _dac_init(device->hw);
}

/**
 * \brief Initialize DAC
 */
int32_t _dac_async_init(struct _dac_async_device *const device, void *const hw)
{
	int32_t init_status;

	ASSERT(device);

	init_status = _dac_init(hw);
	if (init_status) {
		return init_status;
	}

	device->hw = hw;
	_dac_dev   = device;
	NVIC_DisableIRQ(DACC_IRQn);
	NVIC_ClearPendingIRQ(DACC_IRQn);
	NVIC_EnableIRQ(DACC_IRQn);

	return ERR_NONE;
}

/**
 * \brief De-initialize DAC
 */
void _dac_sync_deinit(struct _dac_sync_device *const device)
{
	_dac_deinit(device->hw);
}

/**
 * \brief De-initialize DAC
 */
void _dac_async_deinit(struct _dac_async_device *const device)
{
	NVIC_DisableIRQ(DACC_IRQn);

	_dac_deinit(device->hw);
}

/**
 * \brief Enable DAC Channel
 */
void _dac_sync_enable_channel(struct _dac_sync_device *const device, const uint8_t ch)
{
	_dac_enable_channel(device->hw, ch);
}

/**
 * \brief Enable DAC Channel
 */
void _dac_async_enable_channel(struct _dac_async_device *const device, const uint8_t ch)
{
	_dac_enable_channel(device->hw, ch);
}

/**
 * \brief Disable DAC Channel
 */
void _dac_sync_disable_channel(struct _dac_sync_device *const device, const uint8_t ch)
{
	_dac_disable_channel(device->hw, ch);
}

/**
 * \brief Disable DAC Channel
 */
void _dac_async_disable_channel(struct _dac_async_device *const device, const uint8_t ch)
{
	_dac_disable_channel(device->hw, ch);
}

/**
 * \brief Checks if DAC channel is enabled
 */
bool _dac_sync_is_channel_enable(struct _dac_sync_device *const device, const uint8_t ch)
{
	return _dac_is_channel_enabled(device->hw, ch);
}

/**
 * \brief Checks if DAC channel is enabled
 */
bool _dac_async_is_channel_enable(struct _dac_async_device *const device, const uint8_t ch)
{
	return _dac_is_channel_enabled(device->hw, ch);
}

/**
 * \brief write synchronous DAC data for output
 */
void _dac_sync_write_data(struct _dac_sync_device *const device, const uint16_t data, const uint8_t ch)
{
	ASSERT(device);
	void *hw = device->hw;

	if (ch == 0) {
		while (!(hri_dacc_get_CHSR_DACRDY0_bit(hw)))
			;
		hri_dacc_write_CDR_reg(hw, 0, (hri_dacc_cdr_reg_t)data);
	} else if (ch == 1) {
		while (!(hri_dacc_get_CHSR_DACRDY1_bit(hw)))
			;
		hri_dacc_write_CDR_reg(hw, 1, (hri_dacc_cdr_reg_t)data);
	} else {
		ASSERT(false);
	}
}

/**
 * \brief write DAC data for output
 */
void _dac_async_write_data(struct _dac_async_device *const device, const uint16_t data, const uint8_t ch)
{
	ASSERT(device);
	void *hw = device->hw;

	if (ch == 0) {
		hri_dacc_write_CDR_reg(hw, 0, (hri_dacc_cdr_reg_t)data);
	} else if (ch == 1) {
		hri_dacc_write_CDR_reg(hw, 1, (hri_dacc_cdr_reg_t)data);
	} else {
		ASSERT(false);
	}
}

/**
 * \brief Enable/disable DAC interrupt
 */
void _dac_async_set_irq_state(struct _dac_async_device *const device, const enum _dac_callback_type type,
                              const bool state)
{
	ASSERT(device);
	void *hw = device->hw;

	if (state) {
		if (DAC_DEVICE_CONVERSION_DONE_CB == type) {
			hri_dacc_set_IMR_EOC0_bit(hw);
			hri_dacc_set_IMR_EOC1_bit(hw);
		} else if (DAC_DEVICE_ERROR_CB != type) {
			ASSERT(false);
		}
	} else {
		ASSERT(false);
	}
}

/**
 * \internal Initialize DAC
 *
 * \param[in] hw The pointer to DAC hardware instance
 *
 * \return The status of initialization
 */
static int32_t _dac_init(void *const hw)
{
	ASSERT(hw);

	hri_dacc_write_CR_reg(hw, DACC_CR_SWRST);
	hri_dacc_write_CHDR_reg(hw, DACC_CHDR_Msk);
	_dac_write_protection_disable(hw);
	hri_dacc_write_MR_reg(hw, _dac.mr);
	hri_dacc_write_TRIGR_reg(hw, _dac.trigr);

	return ERR_NONE;
}

/**
 * \internal De-initialize DAC
 *
 * \param[in] hw The pointer to DAC hardware instance
 */
static inline void _dac_deinit(void *const hw)
{
	ASSERT(hw);

	hri_dacc_write_CR_reg(hw, DACC_CR_SWRST);
}

/**
 * \internal Enable DAC channel
 *
 * \param[in] hw The pointer to hardware instance
 * \param[in] ch The channel to enable
 */
static inline void _dac_enable_channel(void *const hw, const uint8_t ch)
{
	ASSERT(hw);

	_dac_write_protection_disable(hw);
	if (ch == 0) {
		hri_dacc_write_CHER_reg(hw, DACC_CHER_CH0);
	} else if (ch == 1) {
		hri_dacc_write_CHER_reg(hw, DACC_CHER_CH1);
	} else {
		ASSERT(false);
	}
}

/**
 * \internal Disable DAC channel
 *
 * \param[in] hw The pointer to hardware instance
 * \param[in] ch The channel to disable
 */
static inline void _dac_disable_channel(void *const hw, const uint8_t ch)
{
	ASSERT(hw);

	_dac_write_protection_disable(hw);
	if (ch == 0) {
		hri_dacc_write_CHDR_reg(hw, DACC_CHDR_CH0);
	} else if (ch == 1) {
		hri_dacc_write_CHDR_reg(hw, DACC_CHDR_CH1);
	} else {
		ASSERT(false);
	}
}

/**
 * \internal Checks if DAC channel is enabled
 *
 * \param[in] hw The pointer to hardware instance
 * \param[in] ch The channel to check
 */
static inline bool _dac_is_channel_enabled(void *const hw, const uint8_t ch)
{
	ASSERT(hw);

	if (ch == 0) {
		return hri_dacc_get_CHSR_CH0_bit(hw);
	} else if (ch == 1) {
		return hri_dacc_get_CHSR_CH1_bit(hw);
	} else {
		return false;
	}
}

/**
 * \internal Disable DAC write protection
 *
 * \param[in] hw The pointer to hardware instance
 */
static inline void _dac_write_protection_disable(void *const hw)
{
	ASSERT(hw);
	hri_dacc_write_WPMR_reg(hw, (hri_dacc_wpmr_reg_t)DACC_WPMR_WPKEY_PASSWD);
}
