
################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ifdef SystemRoot
	SHELL = cmd.exe
	MK_DIR = mkdir
else
	ifeq ($(shell uname), Linux)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), CYGWIN)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW32)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW64)
		MK_DIR = mkdir -p
	endif
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
hpl/efc \
examples \
stdio_redirect/keil \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0 \
spi_nor_flash/n25q256a \
hal/src \
stdio_redirect \
hpl/xdmac \
hal/utils/src \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/RVDS/ARM_CM7 \
hpl/rtc \
hpl/spi \
hpl/usart \
samv71b/armcc/samv71b/armcc/arm \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/MemMang \
spi_nor_flash \
hpl/pmc \
hpl/qspi \
hpl/mcan \
hpl/twihs \
hpl/core

# List the object files
OBJS +=  \
hal/src/hal_io.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/list.o \
hpl/qspi/hpl_qspi.o \
spi_nor_flash_main.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/event_groups.o \
hal/src/hal_calendar.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/croutine.o \
stdio_redirect/stdio_io.o \
atmel_start.o \
hpl/usart/hpl_usart.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/stream_buffer.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/MemMang/heap_1.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/queue.o \
hal/src/hal_spi_m_sync.o \
hal/src/hal_can_async.o \
hal/src/hal_i2c_m_sync.o \
hpl/efc/hpl_efc.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/timers.o \
hal/src/hal_delay.o \
hpl/spi/hpl_spi.o \
hpl/core/hpl_init.o \
hal/utils/src/utils_list.o \
hal/src/hal_usart_os.o \
hal/utils/src/utils_assert.o \
spi_nor_flash/n25q256a/n25q256a.o \
hpl/mcan/hpl_mcan.o \
hpl/pmc/hpl_pmc.o \
stdio_redirect/keil/Retarget.o \
stdio_start.o \
hal/src/hal_flash.o \
hal/src/hal_init.o \
hpl/twihs/hpl_twihs.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.o \
rtos_start.o \
samv71b/armcc/samv71b/armcc/arm/startup_SAMV71.o \
hpl/rtc/hpl_rtc.o \
examples/driver_examples.o \
driver_init.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/RVDS/ARM_CM7/port.o \
main.o \
hal/utils/src/utils_ringbuffer.o \
hal/src/hal_gpio.o \
hal/src/hal_qspi_sync.o \
hal/utils/src/utils_event.o \
hal/src/hal_sleep.o \
spi_nor_flash/spi_nor_flash.o \
hpl/pmc/hpl_sleep.o \
hpl/core/hpl_core_m7_base.o \
samv71b/armcc/samv71b/armcc/arm/system_SAMV71.o \
hal/src/hal_atomic.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/tasks.o \
hpl/xdmac/hpl_xdmac.o

OBJS_AS_ARGS +=  \
"hal/src/hal_io.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/list.o" \
"hpl/qspi/hpl_qspi.o" \
"spi_nor_flash_main.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/event_groups.o" \
"hal/src/hal_calendar.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/croutine.o" \
"stdio_redirect/stdio_io.o" \
"atmel_start.o" \
"hpl/usart/hpl_usart.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/stream_buffer.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/MemMang/heap_1.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/queue.o" \
"hal/src/hal_spi_m_sync.o" \
"hal/src/hal_can_async.o" \
"hal/src/hal_i2c_m_sync.o" \
"hpl/efc/hpl_efc.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/timers.o" \
"hal/src/hal_delay.o" \
"hpl/spi/hpl_spi.o" \
"hpl/core/hpl_init.o" \
"hal/utils/src/utils_list.o" \
"hal/src/hal_usart_os.o" \
"hal/utils/src/utils_assert.o" \
"spi_nor_flash/n25q256a/n25q256a.o" \
"hpl/mcan/hpl_mcan.o" \
"hpl/pmc/hpl_pmc.o" \
"stdio_redirect/keil/Retarget.o" \
"stdio_start.o" \
"hal/src/hal_flash.o" \
"hal/src/hal_init.o" \
"hpl/twihs/hpl_twihs.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.o" \
"rtos_start.o" \
"samv71b/armcc/samv71b/armcc/arm/startup_SAMV71.o" \
"hpl/rtc/hpl_rtc.o" \
"examples/driver_examples.o" \
"driver_init.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/RVDS/ARM_CM7/port.o" \
"main.o" \
"hal/utils/src/utils_ringbuffer.o" \
"hal/src/hal_gpio.o" \
"hal/src/hal_qspi_sync.o" \
"hal/utils/src/utils_event.o" \
"hal/src/hal_sleep.o" \
"spi_nor_flash/spi_nor_flash.o" \
"hpl/pmc/hpl_sleep.o" \
"hpl/core/hpl_core_m7_base.o" \
"samv71b/armcc/samv71b/armcc/arm/system_SAMV71.o" \
"hal/src/hal_atomic.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/tasks.o" \
"hpl/xdmac/hpl_xdmac.o"

# List the dependency files
DEPS := $(OBJS:%.o=%.d)

DEPS_AS_ARGS +=  \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/event_groups.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/croutine.d" \
"hal/src/hal_calendar.d" \
"stdio_redirect/stdio_io.d" \
"hal/utils/src/utils_event.d" \
"hal/src/hal_io.d" \
"spi_nor_flash_main.d" \
"hpl/qspi/hpl_qspi.d" \
"hpl/usart/hpl_usart.d" \
"hpl/xdmac/hpl_xdmac.d" \
"hal/src/hal_i2c_m_sync.d" \
"main.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/timers.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.d" \
"hpl/efc/hpl_efc.d" \
"hal/src/hal_spi_m_sync.d" \
"hal/src/hal_usart_os.d" \
"hal/utils/src/utils_list.d" \
"spi_nor_flash/n25q256a/n25q256a.d" \
"hal/utils/src/utils_assert.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/stream_buffer.d" \
"hal/src/hal_delay.d" \
"hpl/core/hpl_init.d" \
"hpl/spi/hpl_spi.d" \
"hal/src/hal_flash.d" \
"stdio_start.d" \
"samv71b/armcc/samv71b/armcc/arm/startup_SAMV71.d" \
"hpl/twihs/hpl_twihs.d" \
"hal/src/hal_init.d" \
"hpl/mcan/hpl_mcan.d" \
"stdio_redirect/keil/Retarget.d" \
"hpl/pmc/hpl_pmc.d" \
"hal/src/hal_can_async.d" \
"driver_init.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/MemMang/heap_1.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/RVDS/ARM_CM7/port.d" \
"rtos_start.d" \
"examples/driver_examples.d" \
"hpl/rtc/hpl_rtc.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/list.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/queue.d" \
"hal/src/hal_qspi_sync.d" \
"hal/src/hal_sleep.d" \
"hal/utils/src/utils_ringbuffer.d" \
"hal/src/hal_gpio.d" \
"atmel_start.d" \
"hal/src/hal_atomic.d" \
"samv71b/armcc/samv71b/armcc/arm/system_SAMV71.d" \
"hpl/core/hpl_core_m7_base.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/tasks.d" \
"spi_nor_flash/spi_nor_flash.d" \
"hpl/pmc/hpl_sleep.d"

OUTPUT_FILE_NAME :=AtmelStart
QUOTE := "
OUTPUT_FILE_PATH +=$(OUTPUT_FILE_NAME).elf
OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_FILE_NAME).elf

vpath %.c ../
vpath %.s ../
vpath %.S ../

# All Target
all: $(SUB_DIRS) $(OUTPUT_FILE_PATH)

# Linker target

$(OUTPUT_FILE_PATH): $(OBJS)
	@echo Building target: $@
	@echo Invoking: ARMCC Linker
	$(QUOTE)armlink$(QUOTE) --ro-base 0x00000000 --entry 0x00000000 --rw-base 0x20000000 --entry Reset_Handler --first __Vectors \
--strict --summary_stderr --info summarysizes --map --xref --callgraph --symbols \
--info sizes --info totals --info unused --info veneers --list $(OUTPUT_FILE_NAME).map \
-o $(OUTPUT_FILE_NAME).elf --cpu Cortex-M7.fp.dp \
$(OBJS_AS_ARGS)

	@echo Finished building target: $@

# Compiler target(s)




%.o: %.c
	@echo Building file: $<
	@echo ARMCC Compiler
	$(QUOTE)armcc$(QUOTE) --c99 -c -DDEBUG -O1 -g --apcs=interwork --split_sections --cpu Cortex-M7.fp.dp -D__SAMV71Q21B__ \
-I"../" -I"../config" -I"../examples" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/core" -I"../hpl/mcan" -I"../hpl/nvmctrl" -I"../hpl/pio" -I"../hpl/pmc" -I"../hpl/qspi" -I"../hpl/rtc" -I"../hpl/spi" -I"../hpl/twihs" -I"../hpl/usart" -I"../hpl/xdmac" -I"../hri" -I"../" -I"../config" -I"../thirdparty/RTOS" -I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0" -I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/include" -I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/RVDS/ARM_CM7" -I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0/module_config" -I"../" -I"../config" -I"../stdio_redirect" -I"../" -I"../config" -I"../spi_nor_flash" -I"../spi_nor_flash/n25q256a" -I"../" -I"../CMSIS/Core/Include" -I"../samv71b/include"  \
--depend "$@"  -o "$@" "$<"

	@echo Finished building: $<

%.o: %.s
	@echo Building file: $<
	@echo ARMCC Assembler
	$(QUOTE)armasm$(QUOTE) -g --apcs=interwork --cpu Cortex-M7.fp.dp --pd "D__SAMV71Q21B__ SETA 1" \
-I"../" -I"../config" -I"../examples" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/core" -I"../hpl/mcan" -I"../hpl/nvmctrl" -I"../hpl/pio" -I"../hpl/pmc" -I"../hpl/qspi" -I"../hpl/rtc" -I"../hpl/spi" -I"../hpl/twihs" -I"../hpl/usart" -I"../hpl/xdmac" -I"../hri" -I"../" -I"../config" -I"../thirdparty/RTOS" -I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0" -I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/include" -I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/RVDS/ARM_CM7" -I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0/module_config" -I"../" -I"../config" -I"../stdio_redirect" -I"../" -I"../config" -I"../spi_nor_flash" -I"../spi_nor_flash/n25q256a" -I"../" -I"../CMSIS/Core/Include" -I"../samv71b/include"  \
--depend "$(@:%.o=%.d)" -o "$@" "$<"

	@echo Finished building: $<

%.o: %.S
	@echo Building file: $<
	@echo ARMCC Preprocessing Assembler
	$(QUOTE)armcc$(QUOTE) --c99 -c -DDEBUG -O1 -g --apcs=interwork --split_sections --cpu Cortex-M7.fp.dp -D__SAMV71Q21B__ \
-I"../" -I"../config" -I"../examples" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/core" -I"../hpl/mcan" -I"../hpl/nvmctrl" -I"../hpl/pio" -I"../hpl/pmc" -I"../hpl/qspi" -I"../hpl/rtc" -I"../hpl/spi" -I"../hpl/twihs" -I"../hpl/usart" -I"../hpl/xdmac" -I"../hri" -I"../" -I"../config" -I"../thirdparty/RTOS" -I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0" -I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/include" -I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/RVDS/ARM_CM7" -I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0/module_config" -I"../" -I"../config" -I"../stdio_redirect" -I"../" -I"../config" -I"../spi_nor_flash" -I"../spi_nor_flash/n25q256a" -I"../" -I"../CMSIS/Core/Include" -I"../samv71b/include"  \
--depend "$@"  -o "$@" "$<"

	@echo Finished building: $<

# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

$(SUB_DIRS):
	$(MK_DIR) "$@"

clean:
	rm -f $(OBJS_AS_ARGS)
	rm -f $(OUTPUT_FILE_PATH)
	rm -f $(DEPS_AS_ARGS)
	rm -f $(OUTPUT_FILE_NAME).map $(OUTPUT_FILE_NAME).elf
