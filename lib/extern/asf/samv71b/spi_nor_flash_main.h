/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#ifndef _SPI_NOR_FLASH_MAIN_H_INCLUDED
#define _SPI_NOR_FLASH_MAIN_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <n25q256a.h>

extern struct spi_nor_flash *SPI_NOR_FLASH_0;

/**
 * \brief Initialize Temperature Sensors
 */
void spi_nor_flash_init(void);

#ifdef __cplusplus
}
#endif

#endif // _SPI_NOR_FLASH_MAIN_H_INCLUDED
