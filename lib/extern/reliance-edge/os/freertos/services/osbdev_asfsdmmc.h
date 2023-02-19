/*             ----> DO NOT REMOVE THE FOLLOWING NOTICE <----

                  Copyright (c) 2014-2022 Tuxera US Inc.
                      All Rights Reserved Worldwide.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; use version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but "AS-IS," WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
/*  Businesses and individuals that for commercial or other reasons cannot
    comply with the terms of the GPLv2 license must obtain a commercial
    license before incorporating Reliance Edge into proprietary software
    for distribution in any form.

    Visit https://www.tuxera.com/products/reliance-edge/ for more information.
*/
/** @brief
    @file FreeRTOS block device implementation; see osbdev.c for details.
*/
#ifndef OSBDEV_ASFSDMMC_H
#define OSBDEV_ASFSDMMC_H


#include <task.h>

#include <conf_sd_mmc.h>
#include <sd_mmc.h>

/*  sd_mmc_mem_2_ram_multi() and sd_mmc_ram_2_mem_multi() use an unsigned
    16-bit value to specify the sector count, so no transfer can be larger
    than UINT16_MAX sectors.
*/
#define MAX_SECTOR_TRANSFER UINT16_MAX


/** @brief Initialize a disk.

    @param bVolNum  The volume number of the volume whose block device is being
                    initialized.
    @param mode     The open mode, indicating the type of access required.

    @return A negated ::REDSTATUS code indicating the operation result.

    @retval 0           Operation was successful.
    @retval -RED_EIO    A disk I/O error occurred.
    @retval -RED_EROFS  The device is read-only media and write access was
                        requested.
*/
static REDSTATUS DiskOpen(
    uint8_t         bVolNum,
    BDEVOPENMODE    mode)
{
    REDSTATUS       ret = 0;
    uint32_t        ulTries;
    sd_mmc_err_t    mmcRet;
    /*  Note: Assuming the volume number is the same as the SD card slot.  The
        ASF SD/MMC driver supports two SD slots.  This implementation will need
        to be modified if multiple volumes share a single SD card.
    */

    /*  The first time the disk is opened, the SD card can take a while to get
        ready, in which time sd_mmc_test_unit_ready() returns either CTRL_BUSY
        or CTRL_NO_PRESENT.  Try numerous times, waiting half a second after
        each failure.  Empirically, this has been observed to succeed on the
        second try, so trying 10x more than that provides a margin of error.
    */
    for(ulTries = 0U; ulTries < 20U; ulTries++)
    {
        mmcRet = sd_mmc_check(bVolNum);
        if((mmcRet != SD_MMC_INIT_ONGOING) && (mmcRet != SD_MMC_ERR_NO_CARD))
        {
            break;
        }
    }

    if(mmcRet != SD_MMC_OK) {
        ret = -RED_EIO;
    }

    return ret;
}


/** @brief Uninitialize a disk.

    @param bVolNum  The volume number of the volume whose block device is being
                    uninitialized.

    @return A negated ::REDSTATUS code indicating the operation result.

    @retval 0   Operation was successful.
*/
static REDSTATUS DiskClose(
    uint8_t     bVolNum)
{
    (void)bVolNum;
    return 0;
}


/** @brief Return the disk geometry.

    @param bVolNum  The volume number of the volume whose block device geometry
                    is being queried.
    @param pInfo    On successful return, populated with the geometry of the
                    block device.

    @return A negated ::REDSTATUS code indicating the operation result.

    @retval 0   Operation was successful.
*/
static REDSTATUS DiskGetGeometry(
    uint8_t     bVolNum,
    BDEVINFO   *pInfo)
{
    uint32_t capacity = sd_mmc_get_capacity(bVolNum);

    /*  The ASF SD/MMC driver only supports 512-byte sectors.

        Note: ulSectorLast is the last addressable sector, need +1 to
        convert to sector count.  The uint64_t cast is for the edge case
        where ulSectorLast == UINT32_MAX.
    */
    pInfo->ulSectorSize = 512U;
    pInfo->ullSectorCount = capacity / pInfo->ulSectorSize;

    return 0;
}


/** @brief Read sectors from a disk.

    @param bVolNum          The volume number of the volume whose block device
                            is being read from.
    @param ullSectorStart   The starting sector number.
    @param ulSectorCount    The number of sectors to read.
    @param pBuffer          The buffer into which to read the sector data.

    @return A negated ::REDSTATUS code indicating the operation result.

    @retval 0           Operation was successful.
    @retval -RED_EIO    A disk I/O error occurred.
*/
static REDSTATUS DiskRead(
    uint8_t     bVolNum,
    uint64_t    ullSectorStart,
    uint32_t    ulSectorCount,
    void       *pBuffer)
{
    sd_mmc_err_t    ret;

    ret = sd_mmc_init_read_blocks(bVolNum, ullSectorStart, ulSectorCount);
    if(ret != SD_MMC_OK)
        return -RED_EIO;

    ret = sd_mmc_start_read_blocks(pBuffer, ulSectorCount);
    if(ret != SD_MMC_OK)
        return -RED_EIO;

    ret = sd_mmc_wait_end_of_read_blocks(false);
    if(ret != SD_MMC_OK)
        return -RED_EIO;

    return 0;
}


#if REDCONF_READ_ONLY == 0

/** @brief Write sectors to a disk.

    @param bVolNum          The volume number of the volume whose block device
                            is being written to.
    @param ullSectorStart   The starting sector number.
    @param ulSectorCount    The number of sectors to write.
    @param pBuffer          The buffer from which to write the sector data.

    @return A negated ::REDSTATUS code indicating the operation result.

    @retval 0           Operation was successful.
    @retval -RED_EIO    A disk I/O error occurred.
*/
static REDSTATUS DiskWrite(
    uint8_t         bVolNum,
    uint64_t        ullSectorStart,
    uint32_t        ulSectorCount,
    const void     *pBuffer)
{
    sd_mmc_err_t    ret;

    ret = sd_mmc_init_write_blocks(bVolNum, ullSectorStart, ulSectorCount);
    if(ret != SD_MMC_OK)
        return -RED_EIO;

    ret = sd_mmc_start_write_blocks(pBuffer, ulSectorCount);
    if(ret != SD_MMC_OK)
        return -RED_EIO;

    ret = sd_mmc_wait_end_of_write_blocks(false);
    if(ret != SD_MMC_OK)
        return -RED_EIO;

    return 0;
}


/** @brief Flush any caches beneath the file system.

    @param bVolNum  The volume number of the volume whose block device is being
                    flushed.

    @return A negated ::REDSTATUS code indicating the operation result.

    @retval 0           Operation was successful.
    @retval -RED_EIO    A disk I/O error occurred.
*/
static REDSTATUS DiskFlush(
    uint8_t     bVolNum)
{
    return 0;
}

#endif /* REDCONF_READ_ONLY == 0 */


#endif /* OSBDEV_ASFSDMMC_H */

