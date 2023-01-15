/**
 * \file
 *
 * \brief I/O I2C related functionality implementation.
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
#include <hal_i2c_m_os.h>
#include <utils.h>
#include <utils_assert.h>

/**
 * \brief Driver version
 */
#define I2C_M_OS_DRIVER_VERSION 0x00000001u

/**
 * \brief Callback function for tx complete
 */
static void i2c_m_os_tx_complete(struct _i2c_m_async_device *const i2c_dev)
{
	struct i2c_m_os_desc *i2c = CONTAINER_OF(i2c_dev, struct i2c_m_os_desc, device);

	if (!(i2c_dev->service.msg.flags & I2C_M_BUSY)) {
		sem_up(&i2c->xfer_sem);
	}
}

/**
 * \brief Callback function for rx complete
 */
static void i2c_m_os_rx_complete(struct _i2c_m_async_device *const i2c_dev)
{
	struct i2c_m_os_desc *i2c = CONTAINER_OF(i2c_dev, struct i2c_m_os_desc, device);

	if (!(i2c_dev->service.msg.flags & I2C_M_BUSY)) {
		sem_up(&i2c->xfer_sem);
	}
}

/**
 * \brief Callback function for I2C error
 */
static void i2c_m_os_error(struct _i2c_m_async_device *const i2c_dev, int32_t error)
{
	struct i2c_m_os_desc *i2c = CONTAINER_OF(i2c_dev, struct i2c_m_os_desc, device);

	i2c->error = error;
	sem_up(&i2c->xfer_sem);
}

/**
 * \brief Rtos version of I2C I/O read
 */
static int32_t i2c_m_os_read(struct io_descriptor *const io, uint8_t *buf, const uint16_t n)
{
	struct i2c_m_os_desc *i2c = CONTAINER_OF(io, struct i2c_m_os_desc, io);
	struct _i2c_m_msg     msg;
	int32_t               ret;

	msg.addr   = i2c->slave_addr;
	msg.len    = n;
	msg.flags  = I2C_M_STOP | I2C_M_RD;
	msg.buffer = buf;

	/* start transfer then return */
	i2c->error = 0;
	ret        = _i2c_m_async_transfer(&i2c->device, &msg);

	if (ret != 0) {
		/* error occurred */
		return ret;
	}

	if (sem_down(&i2c->xfer_sem, ~0) < 0) {
		return ERR_TIMEOUT;
	}

	if (i2c->error) {
		return ERR_IO;
	}

	return (int32_t)n;
}

/**
 * \brief Rtos version of I2C I/O write
 */
static int32_t i2c_m_os_write(struct io_descriptor *const io, const uint8_t *buf, const uint16_t n)
{
	struct i2c_m_os_desc *i2c = CONTAINER_OF(io, struct i2c_m_os_desc, io);
	struct _i2c_m_msg     msg;
	int32_t               ret;

	msg.addr   = i2c->slave_addr;
	msg.len    = n;
	msg.flags  = I2C_M_STOP;
	msg.buffer = (uint8_t *)buf;

	/* start transfer then return */
	i2c->error = 0;
	ret        = _i2c_m_async_transfer(&i2c->device, &msg);

	if (ret != 0) {
		/* error occurred */
		return ret;
	}

	if (sem_down(&i2c->xfer_sem, ~0) < 0) {
		return ERR_TIMEOUT;
	}

	if (i2c->error) {
		return ERR_IO;
	}

	return (int32_t)n;
}

/**
 * \brief Rtos version of i2c initialize
 */
int32_t i2c_m_os_init(struct i2c_m_os_desc *const i2c, void *const hw)
{
	ASSERT(i2c);

	/* Init I/O */
	i2c->io.read  = i2c_m_os_read;
	i2c->io.write = i2c_m_os_write;

	/* Init callbacks */
	_i2c_m_async_register_callback(&i2c->device, I2C_M_ASYNC_DEVICE_TX_COMPLETE, (FUNC_PTR)i2c_m_os_tx_complete);
	_i2c_m_async_register_callback(&i2c->device, I2C_M_ASYNC_DEVICE_RX_COMPLETE, (FUNC_PTR)i2c_m_os_rx_complete);
	_i2c_m_async_register_callback(&i2c->device, I2C_M_ASYNC_DEVICE_ERROR, (FUNC_PTR)i2c_m_os_error);

	sem_init(&i2c->xfer_sem, 0);

	return _i2c_m_async_init(&i2c->device, hw);
}
/**
 * \brief Rtos version Deinitialize i2c master interface
 */
int32_t i2c_m_os_deinit(struct i2c_m_os_desc *const i2c)
{
	ASSERT(i2c);
	sem_deinit(&i2c->xfer_sem);

	i2c->io.read  = NULL;
	i2c->io.write = NULL;

	_i2c_m_async_register_callback(&i2c->device, I2C_M_ASYNC_DEVICE_TX_COMPLETE, NULL);
	_i2c_m_async_register_callback(&i2c->device, I2C_M_ASYNC_DEVICE_RX_COMPLETE, NULL);
	_i2c_m_async_register_callback(&i2c->device, I2C_M_ASYNC_DEVICE_ERROR, NULL);

	return _i2c_m_async_deinit(&i2c->device);
}
/**
 * \brief Rtos version of i2c enable
 */
int32_t i2c_m_os_enable(struct i2c_m_os_desc *const i2c)
{
	ASSERT(i2c);

	if (_i2c_m_async_enable(&i2c->device) < 0) {
		return ERR_DENIED;
	}
	_i2c_m_async_set_irq_state(&i2c->device, I2C_M_ASYNC_DEVICE_TX_COMPLETE, true);
	_i2c_m_async_set_irq_state(&i2c->device, I2C_M_ASYNC_DEVICE_RX_COMPLETE, true);
	_i2c_m_async_set_irq_state(&i2c->device, I2C_M_ASYNC_DEVICE_ERROR, true);
	return 0;
}

/**
 * \brief Rtos version of i2c disable
 */
int32_t i2c_m_os_disable(struct i2c_m_os_desc *const i2c)
{
	ASSERT(i2c);

	if (_i2c_m_async_disable(&i2c->device) < 0) {
		return ERR_BUSY;
	}
	_i2c_m_async_set_irq_state(&i2c->device, I2C_M_ASYNC_DEVICE_TX_COMPLETE, false);
	_i2c_m_async_set_irq_state(&i2c->device, I2C_M_ASYNC_DEVICE_RX_COMPLETE, false);
	_i2c_m_async_set_irq_state(&i2c->device, I2C_M_ASYNC_DEVICE_ERROR, false);
	return 0;
}

/**
 * \brief Rtos version of i2c set slave address
 */
int32_t i2c_m_os_set_slaveaddr(struct i2c_m_os_desc *const i2c, int16_t addr, int32_t addr_len)
{
	return i2c->slave_addr = (addr & 0x3ff) | (addr_len & I2C_M_TEN);
}

/**
 * \brief Rtos version of i2c set baudrate
 */
int32_t i2c_m_os_set_baudrate(struct i2c_m_os_desc *const i2c, uint32_t clkrate, uint32_t baudrate)
{
	return _i2c_m_async_set_baudrate(&i2c->device, clkrate, baudrate);
}

/**
 * \brief Rtos version of i2c transfer
 */
int32_t i2c_m_os_transfer(struct i2c_m_os_desc *const i2c, struct _i2c_m_msg *msg, int n)
{
	int     i;
	int32_t ret;

	for (i = 0; i < n; i++) {
		ret = _i2c_m_async_transfer(&i2c->device, &msg[i]);

		if (ret != 0) {
			/* error occurred */
			return ret;
		}

		if (0 != sem_down(&i2c->xfer_sem, ~0)) {
			return ERR_TIMEOUT;
		}
	}

	return n;
}

/**
 * \brief Send stop condition
 */
int32_t i2c_m_os_send_stop(struct i2c_m_os_desc *const i2c)
{
	return _i2c_m_async_send_stop(&i2c->device);
}

/**
 * \brief Retrieve the current driver version
 */
uint32_t i2c_m_os_get_version(void)
{
	return I2C_M_OS_DRIVER_VERSION;
}
