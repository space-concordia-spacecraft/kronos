Utility (libutil)
=================

**3.10.2 (2020-03-18)**

- Improvement: Log: Do not call gs_clock_get_time() from ISR context.

**3.10.1 (2020-01-23)**

- Feature: User space interrupt support for linux sysfs gpio driver.
- Feature: Linux - support for native I2C and SPI driver.
- Improvement: Linux - improved gs_time_sleep_ns() by compensating for signals.

**3.9.3 (2019-10-16)**

- Bug: Fixed check in gs_vmem_cpy() for last entry, checked size - not size (as other VMEM functions).
- Feature: Made gs_lock API public (moved header file to public scope).
- Feature: Added vmem test interface used for production checkout.
- Improvement: log appender hist: added pattern match support.
- Bug: gs_string_match() fixed wildcard bug, preventing a valid match.
- Improvement: Disable stdio buffering from within console thread, so it works with newlib reentrant.
- Feature: Added prototype for gs_stdio_put_direct() and linux implementation.

**3.9.2 (2019-07-03)**

- Feature: Added gs_time_to_string() API.

**3.9.1 (2019-05-16)**

- Improvement: Changed strings in command context to const (prevent modification).

**3.8.1 (2019-04-08)**

- Feature: Added GS_PATH_MAX, GS_PLATFORM_64, gs_snprintf().
- Improvement: Changed logs from ISR to print to stderr, so these can be handled differently.
- Improvement: Aligned help text for log groups and appenders.
- Breaking: Added return value to log appender interface.
- Feature: Added log appender 'clear' command.
- Breaking: Extended vmem interface with a check() function. Changed "size" formatting in vmem list.
- Feature: Added "vmem info" command.

**3.7.2 (2019-03-10)**

- Feature: Added support for setting log levels on command line (linux only).

**3.7.1 (2019-02-26)**

- Breaking: gs_command_init(): removed automatic registration of vmem commands, use gs_vmem_register_commands() to register commands.
- Breaking: fixed bugs in command_gen.py (Command generator JSON -> c/h). The register function will always uses the JSON filename.

**3.6.1 (2019-01-30)**

- Improvement: Added support for stopping console thread.
- Improvement: Enabled hexdump() to dump memory from address 0.
- Breaking: Removed attribute "log_groups" from log group definition, requiring log gruops to be registered to show up in lists.
- Improvement: Updated API documentation.

**3.5.2 (2018-12-19)**

- Improvement: Updated documentation.

**3.5.1 (2018-11-21)**

- Breaking: Re-factored the log framework. Log statements are not affected. Only log configuration/setup.
- Breaking: Removed gs_time_rel_s() and gs_time_rel_s_isr().
- Breaking: Linux, gs_command_register() must always be called to register commands.
- Improvement: Added gs_console_start(), which simplifies console initialization.

**3.4.1 (2018-09-20)**

- Feature: Added gs_thread_create_with_stack, so that stack buffer can be manually allocated.
- Feature: Added gs_string_endswith function.
- Improvement: Added logging functionality from interrupt (ISR) context , e.g. gs_log_isr().
- Feature: Added gs_crc8 API.
- Improvement: Increased accepted number commands arguments from 25 to 30.
- Feature: Added gs_thread_join() - primarily for testing.
- Feature: Added support for logging of gosh commands. Default logger available.

**3.3.2 (2018-08-13)**

- Feature: Added unistd::gs_getcwd().
- Feature: GOSH/command added support for specifying mandatory and optional arguments.
- Feature: GOSH/command added support for other IO streams than stdio.
- Feature: Added simple log file store.
- Breaking: GOSH/command, changed gs_command_context_t, gs_command_t and 'complete' callback.
- Breaking: Removed driver_debug API - use Log API instead.

**3.2.1 (2018-05-30)**

- Breaking: Changed GPIO ISR callback to make use of context switch.
- Feature: Added 'no check' versions of GPIO get/set for better perfomance.
- Feature: Added microsecond delays since provided timestamp
- Feature: Added bytebuffer API.

**3.1.2 (2018-04-17)**

- Breaking: Changed I2C interface for getting buffers, gs_i2c_slave_get_rx_buf_t / gs_i2c_slave_set_get_rx_buf().
- Feature: Added gs_fletcher16() functions for handling streaming data.
- Feature: re-added deprecated color_printf and GOSH APIs in order to support "old" clients.

**3.0.1 (2018-03-14)**

- Breaking: Replaced LZO with miniz (Zip)
- Feature: GOSH (for local commands) moved to libutil
- Feature: Generic GPIO prototype and x86_64 implementation
- Feature: Additional string functions

**3.0.0 (2017-12-04)**

- Feature: Software watchdog
- Improvement: Updated to LZO 2.10
- Feature: Additional string functions
- Feature: Additional date/time functions
- Feature: Prototypes for stdio

**2.1.0 (2017-07-28)**

- Improvement: Functionality for 8-bit architectures

**2.0.0 (2017-06-23)**

- Improvement: Use gsbuildtools.
- Feature: Added cross-platform API's for various IO functionality, e.g. SPI, I2C, CAN.

**1.0 (2015-05-08)**

- Feature: Allow time to be set on linux
- Feature: Support for liblog
- Feature: LZO re-added and tested on linux
- Feature: VMEM system
- Feature: Added documentation folder in sphinx format
- Feature: High resolution tick timer system
