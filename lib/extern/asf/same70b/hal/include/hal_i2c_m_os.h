/**
 * \file
 *
 * \brief I2C Hardware Abstraction Layer(HAL) declaration.
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _HAL_I2C_M_OS_H_INCLUDED
#define _HAL_I2C_M_OS_H_INCLUDED

#include <hpl_i2c_m_async.h>
#include <hal_io.h>
#include <hal_rtos.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup doc_driver_hal_i2c_master_os
 *
 * @{
 */

/**
 * \brief Forward declaration of the descriptor type
 */
struct i2c_m_os_desc;

/**
 * \brief I2C descriptor structure, embed i2c_device & i2c_interface
 */
struct i2c_m_os_desc {
	struct _i2c_m_async_device device;
	struct io_descriptor       io;
	sem_t                      xfer_sem;
	uint16_t                   slave_addr;
	uint8_t                    error;
};

/**
 * \brief Initialize I2C master interface
 *
 * This function initializes the given I2C descriptor to be used as an RTOS I2C
 * master interface descriptor.
 * It checks if the given hardware is not initialized and if the given hardware
 * is permitted to be initialized.
 *
 * \param[in] i2c An I2C master descriptor, which is used to communicate
 * \param[in] hw The pointer to hardware instance
 *
 * \return Initialization status.
 * \retval -1 The passed parameters were invalid or already initialized
 * \retval 0 The initialization is completed successfully
 */
int32_t i2c_m_os_init(struct i2c_m_os_desc *const i2c, void *const hw);

/**
 * \brief Deinitialize I2C master interface
 *
 * This function deinitializes the given RTOS I2C master descriptor.
 * It checks if the given hardware is initialized and if the given hardware is
 * permitted to be deinitialized.
 *
 * \param[in] i2c An I2C master descriptor, which is used to communicate through
 *                I2C
 *
 * \return De-initialization status.
 * \retval -1 The passed parameters were invalid or the interface is already
 *            deinitialized
 * \retval 0 The de-initialization is completed successfully
 */
int32_t i2c_m_os_deinit(struct i2c_m_os_desc *const i2c);

/**
 * \brief Set the slave device address
 *
 * This function sets the next transfer target slave I2C device address.
 * It takes no effect to any already started access.
 *
 * \param[in] i2c An I2C master descriptor, which is used to communicate through
 *                I2C
 * \param[in] addr The slave address to access
 * \param[in] addr_len The slave address length, can be I2C_M_TEN or I2C_M_SEVEN
 *
 * \return Masked slave address, the mask is a maximum 10-bit address, and the 10th
 *         bit is set if a 10-bit address is used
 */
int32_t i2c_m_os_set_slaveaddr(struct i2c_m_os_desc *const i2c, int16_t addr, int32_t addr_len);

/**
 * \brief Set baudrate
 *
 * This function sets the I2C master device to a specified baudrate,
 * and it only takes effect when the hardware is disabled
 *
 * \param[in] i2c An I2C master descriptor, which is used to communicate through
 *                I2C
 * \param[in] clkrate Unused parameter, should always be 0
 * \param[in] baudrate The baudrate value set to master
 *
 * \return The status whether successfully set the baudrate
 * \retval -1 The passed parameters were invalid or the device is already enabled
 * \retval 0 The baudrate set is completed successfully
 */
int32_t i2c_m_os_set_baudrate(struct i2c_m_os_desc *const i2c, uint32_t clkrate, uint32_t baudrate);

/**
 * \brief Enable hardware
 *
 * This function enables the I2C master device and then waits for this enabling
 * operation to be done
 *
 * \param[in] i2c An I2C master descriptor, which is used to communicate through
 *                I2C
 *
 * \return The status whether successfully enable the device
 * \retval -1 The passed parameters were invalid or the device enable failed
 * \retval 0 The hardware enabling is completed successfully
 */
int32_t i2c_m_os_enable(struct i2c_m_os_desc *const i2c);

/**
 * \brief Disable hardware
 *
 * This function disables the I2C master device and then waits for this disabling
 * operation to be done
 *
 * \param[in] i2c An I2C master descriptor, which is used to communicate through
 *                I2C
 *
 * \return The status whether successfully disable the device
 * \retval -1 The passed parameters were invalid or the device disable failed
 * \retval 0 The hardware disabling is completed successfully
 */
int32_t i2c_m_os_disable(struct i2c_m_os_desc *const i2c);

/**
 * \brief Async version of transfer message to/from I2C slave
 *
 * This function will transfer messages between the I2C slave and the master.
 * The function will wait for the transfer to complete.
 *
 * \param[in] i2c An I2C master descriptor, which is used to communicate through
 *                I2C
 * \param[in] msg An _i2c_m_msg struct array
 * \param[in] n   The number of msgs in the array
 *
 * \return The status of the operation
 * \retval 0 Operation completed successfully
 * \retval <0 Operation failed
 */
int32_t i2c_m_os_transfer(struct i2c_m_os_desc *const i2c, struct _i2c_m_msg *msg, int n);

/**
 * \brief Generate stop condition on the I2C bus
 *
 * This function will generate a stop condition on the I2C bus
 *
 * \param[in] i2c An I2C master descriptor, which is used to communicate through
 *                I2C
 *
 * \return Operation status
 * \retval 0 Operation executed successfully
 * \retval <0 Operation failed
 */
int32_t i2c_m_os_send_stop(struct i2c_m_os_desc *const i2c);

/**
 * \brief Return I/O descriptor for this I2C instance
 *
 * This function will return a I/O instance for this I2C driver instance
 *
 * \param[in] i2c An I2C master descriptor, which is used to communicate through
 *                I2C
 * \param[in] io A pointer to an I/O descriptor pointer type
 *
 * \return Error code
 * \retval 0 No error detected
 * \retval <0 Error code
 */
static inline int32_t i2c_m_os_get_io(struct i2c_m_os_desc *const i2c, struct io_descriptor **io)
{
	*io = &i2c->io;
	return 0;
}

/**
 * \brief Retrieve the current driver version
 *
 * \return Current driver version.
 */
uint32_t i2c_m_get_version(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif
