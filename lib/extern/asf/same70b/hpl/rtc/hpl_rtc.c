/**
 * \file
 *
 * \brief RTC Driver (Calendar Mode)
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

#include "hpl_calendar.h"
#include <utils_assert.h>
#include "hpl_rtc_config.h"

/* The BCD code shift value */
#define BCD_SHIFT 4

/* The BCD code mask value */
#define BCD_MASK 0xfu

/* The BCD mul/div factor value */
#define BCD_FACTOR 10

/** Pointer to hpl device */
static struct calendar_dev *_rtc_dev = NULL;

#ifndef RTC_MR_VALUE
#define RTC_MR_VALUE                                                                                                   \
	(uint32_t)((RTC_HRMOD << RTC_MR_HRMOD_Pos) | (RTC_PERSIAN << RTC_MR_PERSIAN_Pos)                                   \
	           | (RTC_NEGPPM << RTC_MR_NEGPPM_Pos) | (RTC_MR_CORRECTION(RTC_CORRECTION))                               \
	           | (RTC_HIGHPPM << RTC_MR_HIGHPPM_Pos) | (RTC_MR_OUT0(RTC_OUT0)) | (RTC_MR_OUT1(RTC_OUT1))               \
	           | (RTC_MR_THIGH(RTC_THIGH)) | (RTC_MR_TPERIOD(RTC_TPERIOD)))
#endif

/**
 * \brief Initializes the RTC module with given configurations.
 */
int32_t _calendar_init(struct calendar_dev *const dev)
{
	ASSERT(dev && dev->hw);

	_rtc_dev = dev;
	hri_rtc_write_MR_reg(dev->hw, RTC_MR_VALUE);

	return ERR_NONE;
}

/**
 * \brief De-init the RTC module
 */
int32_t _calendar_deinit(struct calendar_dev *const dev)
{
	ASSERT(dev && dev->hw);

	NVIC_DisableIRQ(RTC_IRQn);
	dev->callback = NULL;

	hri_rtc_write_MR_reg(dev->hw, 0);
	return ERR_NONE;
}

/**
 * \brief Enable the RTC module
 */
int32_t _calendar_enable(struct calendar_dev *const dev)
{
	ASSERT(dev && dev->hw);

	return ERR_NONE;
}

/**
 * \brief Disable the RTC module
 */
int32_t _calendar_disable(struct calendar_dev *const dev)
{
	ASSERT(dev && dev->hw);

	return ERR_NONE;
}

/**
 * \brief Set time for calendar
 */
int32_t _calendar_set_time(struct calendar_dev *const dev, struct calendar_time *const p_calendar_time)
{
	uint32_t ul_time = 0;

	ASSERT(dev && dev->hw);

	/* Hour */
	ul_time |= ((p_calendar_time->hour / BCD_FACTOR) << (RTC_TIMR_HOUR_Pos + BCD_SHIFT))
	           | ((p_calendar_time->hour % BCD_FACTOR) << RTC_TIMR_HOUR_Pos);

	/* Minute */
	ul_time |= ((p_calendar_time->min / BCD_FACTOR) << (RTC_TIMR_MIN_Pos + BCD_SHIFT))
	           | ((p_calendar_time->min % BCD_FACTOR) << RTC_TIMR_MIN_Pos);

	/* Second */
	ul_time |= ((p_calendar_time->sec / BCD_FACTOR) << (RTC_TIMR_SEC_Pos + BCD_SHIFT))
	           | ((p_calendar_time->sec % BCD_FACTOR) << RTC_TIMR_SEC_Pos);

	/* Update time register. Check the spec for the flow. */
	while (!hri_rtc_get_SR_SEC_bit(dev->hw))
		;
	hri_rtc_set_CR_UPDTIM_bit(dev->hw);
	while (!hri_rtc_get_SR_ACKUPD_bit(dev->hw))
		;
	hri_rtc_write_SCCR_reg(dev->hw, RTC_SCCR_ACKCLR);
	hri_rtc_write_TIMR_reg(dev->hw, ul_time);
	hri_rtc_clear_CR_UPDTIM_bit(dev->hw);

	return hri_rtc_get_VER_NVTIM_bit(dev->hw);
}

/**
 * \brief Set date for calendar
 */
int32_t _calendar_set_date(struct calendar_dev *const dev, struct calendar_date *const p_calendar_date)
{
	uint32_t ul_date = 0;

	ASSERT(dev && dev->hw);

	/* Cent */
	ul_date |= ((p_calendar_date->year / BCD_FACTOR / BCD_FACTOR / BCD_FACTOR) << (RTC_CALR_CENT_Pos + BCD_SHIFT)
	            | ((p_calendar_date->year / BCD_FACTOR / BCD_FACTOR) % BCD_FACTOR) << RTC_CALR_CENT_Pos);

	/* Year */
	ul_date |= (((p_calendar_date->year / BCD_FACTOR) % BCD_FACTOR) << (RTC_CALR_YEAR_Pos + BCD_SHIFT))
	           | ((p_calendar_date->year % BCD_FACTOR) << RTC_CALR_YEAR_Pos);

	/* Month */
	ul_date |= ((p_calendar_date->month / BCD_FACTOR) << (RTC_CALR_MONTH_Pos + BCD_SHIFT))
	           | ((p_calendar_date->month % BCD_FACTOR) << RTC_CALR_MONTH_Pos);

	/* Week */
	ul_date |= (0x01 << RTC_CALR_DAY_Pos);

	/* Day */
	ul_date |= ((p_calendar_date->day / BCD_FACTOR) << (RTC_CALR_DATE_Pos + BCD_SHIFT))
	           | ((p_calendar_date->day % BCD_FACTOR) << RTC_CALR_DATE_Pos);

	/* Update calendar register. Check the spec for the flow. */

	while (!hri_rtc_get_SR_SEC_bit(dev->hw))
		;
	hri_rtc_set_CR_UPDCAL_bit(dev->hw);
	while (!hri_rtc_get_SR_ACKUPD_bit(dev->hw))
		;

	hri_rtc_write_SCCR_reg(dev->hw, RTC_SCCR_ACKCLR);
	hri_rtc_write_CALR_reg(dev->hw, ul_date);
	hri_rtc_clear_CR_UPDCAL_bit(dev->hw);

	return hri_rtc_get_VER_NVCAL_bit(dev->hw);
}

/**
 * \brief Get the time for calendar HAL instance and hardware
 *  Retrieve the time from calendar instance.
 */
uint32_t _calendar_get_date_time(struct calendar_dev *const dev, struct calendar_date_time *const date_time)
{
	uint32_t ul_date;
	uint32_t ul_time;
	uint32_t ul_cent;
	uint32_t ul_temp;

	ASSERT(dev && dev->hw);

	/* Get the current date (multiple reads are necessary to insure a stable value). */
	ul_date = hri_rtc_read_CALR_reg(dev->hw);
	while (ul_date != hri_rtc_read_CALR_reg(dev->hw)) {
		ul_date = hri_rtc_read_CALR_reg(dev->hw);
	}

	/* Get year */
	ul_temp = (ul_date & RTC_CALR_CENT_Msk) >> RTC_CALR_CENT_Pos;
	ul_cent = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);
	ul_temp = (ul_date & RTC_CALR_YEAR_Msk) >> RTC_CALR_YEAR_Pos;
	date_time->date.year
	    = (ul_cent * BCD_FACTOR * BCD_FACTOR) + (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);

	/* Get month */
	ul_temp               = (ul_date & RTC_CALR_MONTH_Msk) >> RTC_CALR_MONTH_Pos;
	date_time->date.month = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);

	/* Get day */
	ul_temp             = (ul_date & RTC_CALR_DATE_Msk) >> RTC_CALR_DATE_Pos;
	date_time->date.day = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);

	/* Get the current RTC time (multiple reads are necessary to insure a stable value). */
	ul_time = hri_rtc_read_TIMR_reg(dev->hw);
	while (ul_time != hri_rtc_read_TIMR_reg(dev->hw)) {
		ul_time = hri_rtc_read_TIMR_reg(dev->hw);
	}

	/* Get Hour */
	ul_temp              = (ul_time & RTC_TIMR_HOUR_Msk) >> RTC_TIMR_HOUR_Pos;
	date_time->time.hour = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);

	if ((ul_time & RTC_TIMR_AMPM) == RTC_TIMR_AMPM) {
		date_time->time.hour += 12;
	}

	/* Get Minute */
	ul_temp             = (ul_time & RTC_TIMR_MIN_Msk) >> RTC_TIMR_MIN_Pos;
	date_time->time.min = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);

	/* Second */
	ul_temp             = (ul_time & RTC_TIMR_SEC_Msk) >> RTC_TIMR_SEC_Pos;
	date_time->time.sec = (ul_temp >> BCD_SHIFT) * BCD_FACTOR + (ul_temp & BCD_MASK);

	return ERR_NONE;
}

/**
 * \brief Set compare value for calendar
 */
int32_t _calendar_set_alarm(struct calendar_dev *const dev, struct _calendar_alarm *const alarm)
{
	uint32_t ul_alarm_date = 0, ul_alarm_time = 0;

	ASSERT(dev && dev->hw);

	/* Month alarm setting */
	ul_alarm_date |= ((alarm->datetime.date.month / BCD_FACTOR) << (RTC_CALR_MONTH_Pos + BCD_SHIFT))
	                 | ((alarm->datetime.date.month % BCD_FACTOR) << RTC_CALR_MONTH_Pos);

	/* Day alarm setting */
	ul_alarm_date |= ((alarm->datetime.date.day / BCD_FACTOR) << (RTC_CALR_DATE_Pos + BCD_SHIFT))
	                 | ((alarm->datetime.date.day % BCD_FACTOR) << RTC_CALR_DATE_Pos);

	/* Set alarm */
	hri_rtc_clear_CALALR_reg(dev->hw, (RTC_CALALR_MTHEN | RTC_CALALR_DATEEN));
	hri_rtc_write_CALALR_reg(dev->hw, ul_alarm_date);
	hri_rtc_set_CALALR_reg(dev->hw, (RTC_CALALR_MTHEN | RTC_CALALR_DATEEN));

	/* Hour alarm setting */
	/* If 12-hour mode, set AMPM bit */
	if ((hri_rtc_get_MR_HRMOD_bit(dev->hw)) == RTC_MR_HRMOD) {
		if (alarm->datetime.time.hour > 12) {
			alarm->datetime.time.hour -= 12;
			ul_alarm_time |= RTC_TIMR_AMPM;
		}
	}

	ul_alarm_time |= ((alarm->datetime.time.hour / BCD_FACTOR) << (RTC_TIMR_HOUR_Pos + BCD_SHIFT))
	                 | ((alarm->datetime.time.hour % BCD_FACTOR) << RTC_TIMR_HOUR_Pos);

	/* Minute alarm setting */
	ul_alarm_time |= ((alarm->datetime.time.min / BCD_FACTOR) << (RTC_TIMR_MIN_Pos + BCD_SHIFT))
	                 | ((alarm->datetime.time.min % BCD_FACTOR) << RTC_TIMR_MIN_Pos);

	/* Second alarm setting */
	ul_alarm_time |= ((alarm->datetime.time.sec / BCD_FACTOR) << (RTC_TIMR_SEC_Pos + BCD_SHIFT))
	                 | ((alarm->datetime.time.sec % BCD_FACTOR) << RTC_TIMR_SEC_Pos);

	hri_rtc_clear_TIMALR_reg(dev->hw, (RTC_TIMALR_SECEN | RTC_TIMALR_MINEN | RTC_TIMALR_HOUREN));
	hri_rtc_write_TIMALR_reg(dev->hw, ul_alarm_time);
	hri_rtc_set_TIMALR_reg(dev->hw, (RTC_TIMALR_SECEN | RTC_TIMALR_MINEN | RTC_TIMALR_HOUREN));

	return (hri_rtc_get_VER_NVTIMALR_bit(dev->hw));
}

/**
 * \brief Registers callback for the specified callback type
 */
int32_t _calendar_register_callback(struct calendar_dev *const dev, calendar_drv_cb_alarm_t callback)
{
	ASSERT(dev && dev->hw);

	/* Check callback */
	if (callback != NULL) {
		/* register the callback */
		dev->callback = callback;

		/* enable RTC_IRQn */
		NVIC_ClearPendingIRQ(RTC_IRQn);
		NVIC_EnableIRQ(RTC_IRQn);

		/* enable alarm */
		hri_rtc_set_IMR_ALR_bit(dev->hw);
	} else {
		/* disable alarm */
		hri_rtc_clear_IMR_ALR_bit(dev->hw);

		/* disable RTC_IRQn */
		NVIC_DisableIRQ(RTC_IRQn);
	}

	return ERR_NONE;
}

/**
 * \brief RTC interrupt handler
 */
void RTC_Handler(void)
{
	hri_rtc_clear_IMR_ALR_bit(_rtc_dev->hw);

	/* Read and mask interrupt flag register */
	if (hri_rtc_get_SR_ALARM_bit(_rtc_dev->hw)) {
		hri_rtc_write_SCCR_reg(_rtc_dev->hw, RTC_SCCR_ALRCLR);
		_rtc_dev->callback(_rtc_dev);
	}

	hri_rtc_set_IMR_ALR_bit(_rtc_dev->hw);
}

/**
 * \brief Set calendar IRQ
 */
void _calendar_set_irq(struct calendar_dev *const dev)
{
	(void)dev;
	NVIC_SetPendingIRQ(RTT_IRQn);
}
