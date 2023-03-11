/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

/**
 * Example of using QUAD_SPI_0 to get S25FL1XX status1 value,
 * and check bit 0 which indicate embedded operation is busy or not.
 */
void QUAD_SPI_0_example(void)
{
	uint8_t              status = 0xFF;
	struct _qspi_command cmd    = {
        .inst_frame.bits.inst_en  = 1,
        .inst_frame.bits.data_en  = 1,
        .inst_frame.bits.tfr_type = QSPI_READ_ACCESS,
        .instruction              = 0x05,
        .buf_len                  = 1,
        .rx_buf                   = &status,
    };

	qspi_sync_enable(&QUAD_SPI_0);
	while (status & (1 << 0)) {
		qspi_sync_serial_run_command(&QUAD_SPI_0, &cmd);
	}
	qspi_sync_deinit(&QUAD_SPI_0);
}

/**
 * Example of using CALENDAR_0.
 */
static struct calendar_alarm alarm;

static void alarm_cb(struct calendar_descriptor *const descr)
{
	/* alarm expired */
}

void CALENDAR_0_example(void)
{
	struct calendar_date date;
	struct calendar_time time;

	calendar_enable(&CALENDAR_0);

	date.year  = 2000;
	date.month = 12;
	date.day   = 31;

	time.hour = 12;
	time.min  = 59;
	time.sec  = 59;

	calendar_set_date(&CALENDAR_0, &date);
	calendar_set_time(&CALENDAR_0, &time);

	alarm.cal_alarm.datetime.time.sec = 4;
	alarm.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
	alarm.cal_alarm.mode              = REPEAT;

	calendar_set_alarm(&CALENDAR_0, &alarm, alarm_cb);
}

#define TASK_SPI_0_STACK_SIZE (300 / sizeof(portSTACK_TYPE))
#define TASK_SPI_0_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

static TaskHandle_t xCreatedspi_0Task;
static uint8_t      example_SPI_0[12] = "Hello World!";

/**
 * Example task of using SPI_0 to echo using the IO abstraction.
 */
static void SPI_0_example_task(void *param)
{
	struct io_descriptor *io;
	uint16_t              data;

	(void)param;

	spi_m_os_get_io_descriptor(&SPI_0, &io);
	io_write(io, example_SPI_0, 12);

	for (;;) {
		if (io_read(io, (uint8_t *)&data, 2) == 2) {
			/* read OK, handle data. */;
		} else {
			/* error. */;
		}
		os_sleep(10);
	}
}

/**
 * \brief Create OS task for SPI_0
 */
void task_spi_0_create()
{
	/* Create task for SPI_0 */
	if (xTaskCreate(
	        SPI_0_example_task, "spi_0", TASK_SPI_0_STACK_SIZE, NULL, TASK_SPI_0_STACK_PRIORITY, &xCreatedspi_0Task)
	    != pdPASS) {
		while (1) {
			/* Ideally execution should not reach here. Please checkup stack and FreeRTOS configuration. */
		}
	}
	/* Call vTaskStartScheduler() function in main function. Place vTaskStartScheduler function call after creating all
	 * tasks and before while(1) in main function */
}

#define TASK_I2C_0_STACK_SIZE (300 / sizeof(portSTACK_TYPE))
#define TASK_I2C_0_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

static TaskHandle_t xCreatedi2c_0Task;

/**
 * Example task of using I2C_0 to echo using the IO abstraction.
 * Assume the I2C device is AT30TSE temp sensor on IO1 Xplained Pro connected to
 * XPlained board EXT1.
 */
static void I2C_0_example_task(void *param)
{
	uint16_t data;
	uint8_t  buf[2];
	(void)param;

	i2c_m_os_enable(&I2C_0);

	/* Address of the temp sensor. */
	i2c_m_os_set_slaveaddr(&I2C_0, 0x4f, 0);

	/* Set configuration to use 12-bit temperature */
	buf[0] = 1;
	buf[1] = 3 << 5;
	io_write(&I2C_0.io, buf, 2);
	/* Set to temperature register. */
	buf[0] = 0;
	io_write(&I2C_0.io, buf, 1);

	for (;;) {
		if (io_read(&I2C_0.io, (uint8_t *)&data, 2) == 2) {
			/* read OK, handle data. */;
		} else {
			/* error. */;
		}
		os_sleep(10);
	}
}

/**
 * \brief Create OS task for I2C_0
 */
void task_i2c_0_create()
{
	/* Create task for I2C_0 */
	if (xTaskCreate(
	        I2C_0_example_task, "i2c_0", TASK_I2C_0_STACK_SIZE, NULL, TASK_I2C_0_STACK_PRIORITY, &xCreatedi2c_0Task)
	    != pdPASS) {
		while (1) {
			/* Ideally execution should not reach here. Please checkup stack and FreeRTOS configuration. */
		}
	}
	/* Call vTaskStartScheduler() function in main function. Place vTaskStartScheduler function call after creating all
	 * tasks and before while(1) in main function */
}

#define TASK_USART_0_STACK_SIZE (300 / sizeof(portSTACK_TYPE))
#define TASK_USART_0_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

static TaskHandle_t xCreatedusart_0Task;
static uint8_t      example_USART_0[12] = "Hello World!";

/**
 * Example task of using USART_0 to echo using the IO abstraction.
 */
static void USART_0_example_task(void *param)
{
	struct io_descriptor *io;
	uint16_t              data;

	(void)param;

	usart_os_get_io(&USART_0, &io);
	io_write(io, example_USART_0, 12);

	for (;;) {
		if (io_read(io, (uint8_t *)&data, 2) == 2) {
			io_write(io, (uint8_t *)&data, 2);
		}
		os_sleep(10);
	}
}

/**
 * \brief Create OS task for USART_0
 */
void task_usart_0_create()
{
	/* Create task for USART_0 */
	if (xTaskCreate(USART_0_example_task,
	                "usart_0",
	                TASK_USART_0_STACK_SIZE,
	                NULL,
	                TASK_USART_0_STACK_PRIORITY,
	                &xCreatedusart_0Task)
	    != pdPASS) {
		while (1) {
			/* Please checkup stack and FreeRTOS configuration. */
		}
	}
	/* Call vTaskStartScheduler() function in main function. Place vTaskStartScheduler function call after creating all
	 * tasks and before while(1) in main function */
}

#define TASK_TARGET_IO_STACK_SIZE (300 / sizeof(portSTACK_TYPE))
#define TASK_TARGET_IO_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

static TaskHandle_t xCreatedtarget_ioTask;
static uint8_t      example_TARGET_IO[12] = "Hello World!";

/**
 * Example task of using TARGET_IO to echo using the IO abstraction.
 */
static void TARGET_IO_example_task(void *param)
{
	struct io_descriptor *io;
	uint16_t              data;

	(void)param;

	usart_os_get_io(&TARGET_IO, &io);
	io_write(io, example_TARGET_IO, 12);

	for (;;) {
		if (io_read(io, (uint8_t *)&data, 2) == 2) {
			io_write(io, (uint8_t *)&data, 2);
		}
		os_sleep(10);
	}
}

/**
 * \brief Create OS task for TARGET_IO
 */
void task_target_io_create()
{
	/* Create task for TARGET_IO */
	if (xTaskCreate(TARGET_IO_example_task,
	                "target_io",
	                TASK_TARGET_IO_STACK_SIZE,
	                NULL,
	                TASK_TARGET_IO_STACK_PRIORITY,
	                &xCreatedtarget_ioTask)
	    != pdPASS) {
		while (1) {
			/* Please checkup stack and FreeRTOS configuration. */
		}
	}
	/* Call vTaskStartScheduler() function in main function. Place vTaskStartScheduler function call after creating all
	 * tasks and before while(1) in main function */
}

void CAN_0_tx_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
}
void CAN_0_rx_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t            data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	return;
}

/**
 * Example of using CAN_0 to Encrypt/Decrypt datas.
 */
void CAN_0_example(void)
{
	struct can_message msg;
	struct can_filter  filter;
	uint8_t            send_data[4];
	send_data[0] = 0x00;
	send_data[1] = 0x01;
	send_data[2] = 0x02;
	send_data[3] = 0x03;

	msg.id   = 0x45A;
	msg.type = CAN_TYPE_DATA;
	msg.data = send_data;
	msg.len  = 4;
	msg.fmt  = CAN_FMT_STDID;
	can_async_register_callback(&CAN_0, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_0_tx_callback);
	can_async_enable(&CAN_0);
	/**
	 * CAN_0_tx_callback callback should be invoked after call
	 * can_async_write, and remote device should recieve message with ID=0x45A
	 */
	can_async_write(&CAN_0, &msg);

	msg.id  = 0x100000A5;
	msg.fmt = CAN_FMT_EXTID;
	/**
	 * remote device should recieve message with ID=0x100000A5
	 */
	can_async_write(&CAN_0, &msg);

	/**
	 * CAN_0_rx_callback callback should be invoked after call
	 * can_async_set_filter and remote device send CAN Message with the same
	 * content as the filter.
	 */
	can_async_register_callback(&CAN_0, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_0_rx_callback);
	filter.id   = 0x469;
	filter.mask = 0;
	can_async_set_filter(&CAN_0, 0, CAN_FMT_STDID, &filter);

	filter.id   = 0x10000096;
	filter.mask = 0;
	can_async_set_filter(&CAN_0, 1, CAN_FMT_EXTID, &filter);
}
