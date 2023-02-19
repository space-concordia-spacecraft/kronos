/**
 * \file
 *
 * \brief Embedded Flash Controller(EFC) functionality declaration.
 *
 * Copyright (c) 2016-2019 Microchip Technology Inc. and its subsidiaries.
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
#include <hpl_flash.h>
#include <string.h>
#include <utils_assert.h>
#include <utils.h>
#include <hpl_efc_config.h>

/** Pointer to hpl device */
static struct _flash_device *_efc_dev = NULL;

/**
 * \internal   Erase pages (EPA command)
 * \param[in]  hw      The pointer to hardware instance
 * \param[in]  epa_arg Argument for EPA
 */
static void _efc_erase_pages(void *const hw, const uint32_t epa_arg);
/**
 * \internal   Write a full page (EWP/WP command)
 * \param[in]  hw            The pointer to hardware instance
 * \param[in]  epw           Set to true to do erase and write page
 * \param[in]  page          Page number to write
 * \param[in]  buffer        Pointer to buffer where the data to
 *                           write is stored
 */
static void _efc_write_page(void *const hw, const bool ewp, const uint32_t page,
                            const uint8_t buffer[IFLASH_PAGE_SIZE]);

#ifndef CONF_FLASH_PAGE_SIZE
#define CONF_FLASH_PAGE_SIZE IFLASH_PAGE_SIZE
#endif
#define _IFLASH_PAGE_SIZE (CONF_FLASH_PAGE_SIZE)

/** Size support EPA */
#define _IFLASH_EPA_SIZE (16 * 1024)
/** Number of pages support EPA */
#define _IFLASH_EPA_PAGE (_IFLASH_EPA_SIZE / CONF_FLASH_PAGE_SIZE)
/** Erasing block size */
#define _IFLASH_ERASE_SIZE (CONF_FLASH_ERASING_BLK_SIZE)
/** Number of pages for erasing */
#define _IFLASH_ERASE_UNIT (_IFLASH_ERASE_SIZE / CONF_FLASH_PAGE_SIZE)

#define _efc_in_8k_sect(addr) ((addr) < _IFLASH_EPA_SIZE)
#define _efc_page_in_8k_sect(page) _efc_in_8k_sect(_efc_page_addr(page))
#define _efc_page_aligned(addr) (((addr) & (CONF_FLASH_PAGE_SIZE - 1)) == 0)
#define _efc_2k_aligned(addr) (((addr) & (2048 - 1)) == 0)
#define _efc_4k_aligned(addr) (((addr) & (4096 - 1)) == 0)
#define _efc_8k_aligned(addr) (((addr) & (8192 - 1)) == 0)

#define _efc_page(addr) ((addr) / CONF_FLASH_PAGE_SIZE)
#define _efc_page_addr(page) ((page)*CONF_FLASH_PAGE_SIZE)
#define _efc_page_offset(addr) ((addr) & (CONF_FLASH_PAGE_SIZE - 1))

#define _NEED_BUF                                                                                                      \
	(CONF_FLASH_REWR_UNALIGN_OP                                                                                        \
	 && (CONF_FLASH_REWRITE_ENABLE || CONF_FLASH_UNALIGN_APPEND_ENABLE || CONF_FLASH_UNALIGN_ERASE_ENABLE))
#define _STATIC_BUF (CONF_FLASH_OP_BUF_LOC == 0)
#define _STACK_BUF (CONF_FLASH_OP_BUF_LOC == 1)
#define _NEED_ERASE_BUF (CONF_FLASH_REWRITE_ENABLE || CONF_FLASH_UNALIGN_ERASE_ENABLE)
#if _NEED_ERASE_BUF
#define _BUF_SIZE (_IFLASH_ERASE_SIZE)
#else
#define _BUF_SIZE (_IFLASH_PAGE_SIZE)
#endif
#if _NEED_BUF
#if _STATIC_BUF
static uint32_t _efc_buf32[_BUF_SIZE / 4];
#endif
#define _efc_buf8 ((uint8_t *)_efc_buf32)
#endif

/**
 * \brief Initialize NVM
 */
int32_t _flash_init(struct _flash_device *const device, void *const hw)
{
	ASSERT(device && hw);

	device->hw = hw;

	_efc_dev = device;
	NVIC_DisableIRQ(EFC_IRQn);
	NVIC_ClearPendingIRQ(EFC_IRQn);
	NVIC_EnableIRQ(EFC_IRQn);

	return ERR_NONE;
}

/**
 * \brief De-initialize NVM
 */
void _flash_deinit(struct _flash_device *const device)
{
	ASSERT(device);

	device->hw = NULL;
	NVIC_DisableIRQ(EFC_IRQn);
}

/**
 * \brief Get the flash page size.
 */
uint32_t _flash_get_page_size(struct _flash_device *const device)
{
	(void)device;
	return (uint32_t)IFLASH_PAGE_SIZE;
}

/**
 * \brief Get the numbers of flash page.
 */
uint32_t _flash_get_total_pages(struct _flash_device *const device)
{
	(void)device;
	return (uint32_t)IFLASH_NB_OF_PAGES;
}

/**
 * \brief Get the number of wait states for read and write operations.
 */
uint8_t _flash_get_wait_state(struct _flash_device *const device)
{
	return hri_efc_read_EEFC_FMR_FWS_bf(device->hw);
}

/**
 * \brief Set the number of wait states for read and write operations.
 */
void _flash_set_wait_state(struct _flash_device *const device, uint8_t state)
{
	hri_efc_write_EEFC_FMR_FWS_bf(device->hw, state);
}

/**
 * \brief Reads a number of bytes to a page in the internal Flash.
 */
void _flash_read(struct _flash_device *const device, const uint32_t src_addr, uint8_t *buffer, uint32_t length)
{
	ASSERT(((src_addr + length) < IFLASH_SIZE));

	(void)device;
	memcpy(buffer, (void *)(src_addr + IFLASH_ADDR), length);
}

/**
 * \brief Writes a number of bytes to a page in the internal Flash.
 */
#if CONF_FLASH_REWR_UNALIGN_OP && CONF_FLASH_REWRITE_ENABLE
void _flash_write(struct _flash_device *const device, const uint32_t dst_addr, uint8_t *buffer, uint32_t length)
{
#if _STACK_BUF
	uint32_t _efc_buf32[_BUF_SIZE / 4];
#endif
	uint32_t wr_addr = dst_addr;
	uint32_t dst_end = dst_addr + length;
	uint32_t wr_blk  = _efc_in_8k_sect(dst_end) ? CONF_FLASH_PAGE_SIZE : 8192;
	if (length == 0) {
		return;
	}
	while (wr_addr < dst_end) {
		uint16_t cpy_size = 0;
		uint32_t wr_start = wr_addr & ~(wr_blk - 1);
		uint32_t wr_end   = wr_start + wr_blk;
		if (wr_addr > wr_start) { /* Start address not aligned */
			cpy_size = wr_end - wr_addr;
		}
		if (wr_end > dst_end) { /* End address not aligned */
			if (cpy_size) {
				cpy_size = dst_end - wr_addr;
			} else {
				cpy_size = dst_end - wr_start;
			}
		}
		if (cpy_size) {
			/* Save previous data (whole block) */
			_flash_read(device, wr_start, _efc_buf8, wr_blk);
			/* Modify data */
			memcpy(&_efc_buf8[wr_addr - wr_start], buffer, cpy_size);
			/* Erase block */
			if (wr_blk != CONF_FLASH_PAGE_SIZE) {
				_efc_erase_pages(device->hw, _efc_page(wr_start) | 0x02);
			}
			/* Update buffer pointer */
			buffer += cpy_size;
		} else {
			memcpy(_efc_buf8, buffer, wr_blk);
			/* Update buffer pointer */
			buffer += wr_blk;
		}
		/* Program data */
		if (wr_blk == CONF_FLASH_PAGE_SIZE) { /* EWP */
			_efc_write_page(device->hw, true, _efc_page(wr_addr), _efc_buf8);
		} else {
			for (wr_addr = wr_start; wr_addr < wr_end; wr_addr += CONF_FLASH_PAGE_SIZE) {
				_efc_write_page(device->hw, false, _efc_page(wr_addr), &_efc_buf8[wr_addr - wr_start]);
			}
		}
		wr_addr = wr_end;
	}
}
#else
void _flash_write(struct _flash_device *const device, const uint32_t dst_addr, uint8_t *buffer, uint32_t length)
{
	uint32_t addr;
	/* Address must be aligned */
	ASSERT(_efc_page_aligned(dst_addr) && _efc_page_aligned(dst_addr + length));
	/* Buffer must be aligned */
	ASSERT(((uint32_t)buffer & 0x3) == 0);

	for (addr = dst_addr; addr < dst_addr + length; addr += IFLASH_PAGE_SIZE) {
		_efc_write_page(device->hw, false, _efc_page(addr), &buffer[addr - dst_addr]);
	}
}
#endif

/**
 * \brief Appends a number of bytes in the internal Flash.
 */
#if CONF_FLASH_REWR_UNALIGN_OP && CONF_FLASH_UNALIGN_APPEND_ENABLE
void _flash_append(struct _flash_device *const device, const uint32_t dst_addr, uint8_t *buffer, uint32_t length)
{
#if _STACK_BUF
	uint32_t _efc_buf32[_BUF_SIZE / 4];
#endif
	uint32_t addr = dst_addr;
	uint32_t ps_addr, pn_addr;
	uint32_t prev_length;
	uint32_t total_length;
	uint32_t last_addr_to_program;
	uint16_t bytes_programmed_in_current_page;
	if (length == 0) {
		return;
	}
	prev_length          = 0;
	total_length         = length;
	last_addr_to_program = (dst_addr + length);
	do {
		uint8_t *buf    = buffer;
		uint16_t before = 0, after = 0;
		ps_addr = addr & ~(IFLASH_PAGE_SIZE - 1);
		pn_addr = ps_addr + IFLASH_PAGE_SIZE;
		if (addr > ps_addr) { /* Reserve data before destination */
			before = addr - ps_addr;
			buf    = _efc_buf8;
			memset(buf, 0xFF, before);
		}
		if ((addr + length) < pn_addr) { /* Reserve data after destination */
			after = pn_addr - (addr + length);
			buf   = _efc_buf8;
			memset(&buf[IFLASH_PAGE_SIZE - (after)], 0xFF, after);
		}
		if ((uint32_t)buffer & 0x3) { /* Buffer need align */
			buf = _efc_buf8;
		}
		if (buf == _efc_buf8) { /* Copy data to temp buffer */
			memcpy(&buf[before], buffer, IFLASH_PAGE_SIZE - before - after);
		}
		_efc_write_page(device->hw, false, _efc_page(ps_addr), buf);
		if (pn_addr >= last_addr_to_program) {
			break;
		}
		bytes_programmed_in_current_page = IFLASH_PAGE_SIZE - before - after;
		addr                             = pn_addr;
		buffer += IFLASH_PAGE_SIZE - before;
		prev_length += bytes_programmed_in_current_page;
		length = total_length - prev_length;
	} while (1);
}
#else
void _flash_append(struct _flash_device *const device, const uint32_t dst_addr, uint8_t *buffer, uint32_t length)
{
	uint32_t addr;
	/* Address must be aligned */
	ASSERT(_efc_page_aligned(dst_addr) && _efc_page_aligned(dst_addr + length));
	/* Buffer must be aligned */
	ASSERT(((uint32_t)buffer & 0x3) == 0);

	for (addr = dst_addr; addr < dst_addr + length; addr += IFLASH_PAGE_SIZE) {
		_efc_write_page(device->hw, false, _efc_page(addr), &buffer[addr - dst_addr]);
	}
}
#endif

/**
 * \brief Erase aligned area of internal flash
 */
static void _efc_erase_aligned(void *const hw, uint32_t page, uint32_t page_end)
{
	while (page < page_end) {
		if (page < _efc_page(_IFLASH_EPA_SIZE)) {
			/* Erase by 4 pages */
			_efc_erase_pages(hw, page);
			page += 4;
		} else {
			/* Erase by 16 pages */
			_efc_erase_pages(hw, page | 0x02);
			page += 16;
		}
	}
}

/**
 * \brief Execute erase in the internal flash
 */
#if CONF_FLASH_REWR_UNALIGN_OP && CONF_FLASH_UNALIGN_ERASE_ENABLE
void _flash_erase(struct _flash_device *const device, uint32_t dst_addr, uint32_t page_nums)
{
#if _STACK_BUF
	uint32_t _efc_buf32[_BUF_SIZE / 4];
#endif
	uint32_t page     = _efc_page(dst_addr);
	uint32_t page_end = page + page_nums;
	uint32_t dst_end  = page_end * CONF_FLASH_PAGE_SIZE;
	if (page_nums == 0) {
		return;
	}
	/* Erase block by block */
	while (dst_addr < dst_end) {
		uint32_t erase_blk   = _efc_in_8k_sect(dst_addr) ? 2048 : 8192;
		uint16_t set_size    = 0;
		uint32_t erase_start = dst_addr & ~(erase_blk - 1);
		uint32_t erase_end   = erase_start + erase_blk;
		/* Not aligned:
		 * 1. Start address is not aligned
		 * 2. End address is not aligned
		 */
		if (erase_start < dst_addr) { /* Start address not aligned */
			/* Set erase from start to block end */
			set_size = erase_end - dst_addr;
		}
		if (erase_end > dst_end) { /* End address not aligned */
			if (set_size) {        /* Set erase from start to end */
				set_size = dst_end - dst_addr;
			} else { /* Set erase from block start to end */
				set_size = dst_end - erase_start;
			}
		}
		if (set_size) {
			/* Save previous data (whole block) */
			_flash_read(device, erase_start, _efc_buf8, erase_blk);
			/* Set erased data to 0xFF */
			memset(&_efc_buf8[dst_addr - erase_start], 0xFF, set_size);
		}
		_efc_erase_aligned(device->hw, _efc_page(erase_start), _efc_page(erase_end));
		if (set_size) {
			/* Restore previous data */
			for (dst_addr = erase_start; dst_addr < erase_end; dst_addr += CONF_FLASH_PAGE_SIZE) {
				_efc_write_page(device->hw, false, _efc_page(dst_addr), &_efc_buf8[dst_addr - erase_start]);
			}
		}
		dst_addr = erase_end;
	}
}
#else
void _flash_erase(struct _flash_device *const device, uint32_t dst_addr, uint32_t page_nums)
{
	uint32_t page     = _efc_page(dst_addr);
	uint32_t page_end = page + page_nums;

	/* Address must be aligned */
	ASSERT((_efc_in_8k_sect(dst_addr) && _efc_2k_aligned(dst_addr))
	       || (!_efc_in_8k_sect(dst_addr) && _efc_8k_aligned(dst_addr)));
	ASSERT(
	    (_efc_in_8k_sect(dst_addr + _efc_page_addr(page_nums)) && _efc_2k_aligned(dst_addr + _efc_page_addr(page_nums)))
	    || (!_efc_page_in_8k_sect(dst_addr + _efc_page_addr(page_nums))
	        && _efc_8k_aligned(dst_addr + _efc_page_addr(page_nums))));

	_efc_erase_aligned(device->hw, page, page_end);
}
#endif

static int32_t _efc_flash_lock(struct _flash_device *const device, const uint32_t dst_addr, uint32_t page_nums,
                               bool lock)
{
	/* The page_nums must equals one region pages numbers, and dst_addr must
	 * align with region.
	 */
	if ((page_nums != EFC_PAGES_PR_REGION) || (dst_addr & (EFC_PAGES_PR_REGION * IFLASH_PAGE_SIZE - 1))) {
		return ERR_INVALID_ARG;
	}

	while (!hri_efc_get_EEFC_FSR_FRDY_bit(device->hw)) {
		/* Wait for EFC ready to start a new command */
	}

	hri_efc_write_EEFC_FCR_reg(device->hw,
	                           ((lock) ? EEFC_FCR_FCMD_SLB : EEFC_FCR_FCMD_CLB)
	                               | EEFC_FCR_FARG(dst_addr / IFLASH_PAGE_SIZE) | EEFC_FCR_FKEY_PASSWD);

	while (!hri_efc_get_EEFC_FSR_FRDY_bit(device->hw)) {
		/* Wait for EFC ready to start a new command */
	}
	return (int32_t)(EFC_PAGES_PR_REGION * IFLASH_PAGE_SIZE);
}

/**
 * \brief Execute lock in the internal flash
 */
int32_t _flash_lock(struct _flash_device *const device, const uint32_t dst_addr, uint32_t page_nums)
{
	return _efc_flash_lock(device, dst_addr, page_nums, true);
}

/**
 * \brief Execute unlock in the internal flash
 */
int32_t _flash_unlock(struct _flash_device *const device, const uint32_t dst_addr, uint32_t page_nums)
{
	return _efc_flash_lock(device, dst_addr, page_nums, false);
}

/**
 * \brief check whether the region which is pointed by address
 */
bool _flash_is_locked(struct _flash_device *const device, const uint32_t dst_addr)
{
	uint32_t region;

	/* Get region index for the given Address */
	region = dst_addr & ~(IFLASH_PAGE_SIZE * EFC_PAGES_PR_REGION - 1);
	region = region / (IFLASH_PAGE_SIZE * EFC_PAGES_PR_REGION);

	hri_efc_write_EEFC_FCR_reg(device->hw, EEFC_FCR_FCMD_GLB | EEFC_FCR_FKEY_PASSWD);
	while (!hri_efc_get_EEFC_FSR_FRDY_bit(device->hw)) {
		/* Wait for EFC ready to start a new command */
	}

	if (region >= 32) {
		hri_efc_read_EEFC_FRR_reg(device->hw);
	}

	region = (hri_efc_read_EEFC_FRR_reg(device->hw) >> (region & 0x1F)) & 0x1;

	return (bool)region;
}

/**
 * \brief Enable/disable Flash interrupt
 */
void _flash_set_irq_state(struct _flash_device *const device, const enum _flash_cb_type type, const bool state)
{
	ASSERT(device);

	if (FLASH_DEVICE_CB_READY == type) {
		hri_efc_write_EEFC_FMR_FRDY_bit(device->hw, state);
	} else if (FLASH_DEVICE_CB_ERROR == type) {
		/* Not supported */
	}
}

/**
 * \internal   Erase pages (EPA command)
 * \param[in]  hw      The pointer to hardware instance
 * \param[in]  epa_arg Argument for EPA
 */
RAMFUNC
static void _efc_erase_pages(void *const hw, const uint32_t epa_arg)
{
	while (!hri_efc_get_EEFC_FSR_FRDY_bit(hw)) {
		/* Wait for EFC ready to start a new command */
	}

	hri_efc_write_EEFC_FCR_reg(hw, EEFC_FCR_FCMD_EPA | EEFC_FCR_FARG(epa_arg) | EEFC_FCR_FKEY_PASSWD);
}

/**
 * \internal   Write a full page
 * \param[in]  hw            The pointer to hardware instance
 * \param[in]  epw           Set to true to do erase and write page
 * \param[in]  page          Page number to write
 * \param[in]  buffer        Pointer to buffer where the data to
 *                           write is stored
 */
RAMFUNC
static void _efc_write_page(void *const hw, const bool ewp, const uint32_t page, const uint8_t buffer[IFLASH_PAGE_SIZE])
{
	uint32_t i;

	while (!hri_efc_get_EEFC_FSR_FRDY_bit(hw)) {
		/* Wait for EFC ready to start a new command */
	}

	/**
	 * Data to be programmed in the Flash must be written in an internal latch
	 * buffer before writing the programming command in EEFC_FCR.
	 * Byte and half-word AHB accesses to the latch buffer are not allowed.
	 * Only 32-bit word accesses are supported.
	 */
	for (i = 0; i < IFLASH_PAGE_SIZE; i += 4) {
		*((uint32_t *)(IFLASH_ADDR + (page * EFC_PAGE_SIZE) + i)) = *((uint32_t *)(buffer + i));
	}

	__DSB();
	__DMB();

	/* Erase and write page */
	hri_efc_write_EEFC_FCR_reg(
	    hw, ((ewp) ? EEFC_FCR_FCMD_EWP : EEFC_FCR_FCMD_WP) | EEFC_FCR_FARG(page) | EEFC_FCR_FKEY_PASSWD);
}

/**
 * \brief EFC interrupt handler
 */
void EFC_Handler(void)
{
	void *const hw = _efc_dev->hw;

	if (hri_efc_get_EEFC_FSR_FRDY_bit(hw)) {
		if (NULL != _efc_dev->flash_cb.ready_cb) {
			_efc_dev->flash_cb.ready_cb(_efc_dev);
		}
	}
}
