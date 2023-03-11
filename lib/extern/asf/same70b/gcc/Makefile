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

	ifeq ($(shell uname | cut -d _ -f 1), DARWIN)
		MK_DIR = mkdir -p
	endif
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
hpl/hsmci \
hpl/qspi \
sd_mmc \
examples \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0 \
same70b/gcc \
stdio_redirect/gcc \
hal/src \
stdio_redirect \
hpl/xdmac \
hal/utils/src \
hpl/rtc \
hpl/spi \
hpl/usart \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/MemMang \
same70b/gcc/gcc \
hpl/pmc \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM7 \
hpl/mcan \
hpl/twihs \
hpl/core

# List the object files
OBJS +=  \
hal/src/hal_io.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/list.o \
hpl/qspi/hpl_qspi.o \
hal/src/hal_can_async.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/event_groups.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/MemMang/heap_4.o \
hal/src/hal_calendar.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/croutine.o \
hal/utils/src/utils_event.o \
stdio_redirect/stdio_io.o \
hpl/xdmac/hpl_xdmac.o \
sd_mmc_start.o \
hpl/usart/hpl_usart.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/stream_buffer.o \
hal/utils/src/utils_syscalls.o \
stdio_redirect/gcc/read.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/queue.o \
hal/src/hal_i2c_m_os.o \
hpl/pmc/hpl_sleep.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM7/port.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/timers.o \
hal/src/hal_delay.o \
hpl/spi/hpl_spi.o \
hpl/core/hpl_init.o \
hal/utils/src/utils_list.o \
hal/src/hal_usart_os.o \
hal/utils/src/utils_assert.o \
hpl/mcan/hpl_mcan.o \
hpl/pmc/hpl_pmc.o \
hal/src/hal_spi_m_os.o \
stdio_start.o \
hal/src/hal_init.o \
hpl/twihs/hpl_twihs.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.o \
rtos_start.o \
sd_mmc/sd_mmc.o \
hpl/rtc/hpl_rtc.o \
examples/driver_examples.o \
led_flasher_main.o \
driver_init.o \
same70b/gcc/gcc/startup_same70q21b.o \
same70b/gcc/system_same70q21b.o \
hal/utils/src/utils_ringbuffer.o \
hal/src/hal_gpio.o \
hal/src/hal_qspi_sync.o \
hpl/hsmci/hpl_hsmci.o \
hal/src/hal_mci_sync.o \
hal/src/hal_sleep.o \
atmel_start.o \
hpl/core/hpl_core_m7_base.o \
hal/src/hal_atomic.o \
thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/tasks.o \
stdio_redirect/gcc/write.o

OBJS_AS_ARGS +=  \
"hal/src/hal_io.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/list.o" \
"hpl/qspi/hpl_qspi.o" \
"hal/src/hal_can_async.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/event_groups.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/MemMang/heap_4.o" \
"hal/src/hal_calendar.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/croutine.o" \
"hal/utils/src/utils_event.o" \
"stdio_redirect/stdio_io.o" \
"hpl/xdmac/hpl_xdmac.o" \
"sd_mmc_start.o" \
"hpl/usart/hpl_usart.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/stream_buffer.o" \
"hal/utils/src/utils_syscalls.o" \
"stdio_redirect/gcc/read.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/queue.o" \
"hal/src/hal_i2c_m_os.o" \
"hpl/pmc/hpl_sleep.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM7/port.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/timers.o" \
"hal/src/hal_delay.o" \
"hpl/spi/hpl_spi.o" \
"hpl/core/hpl_init.o" \
"hal/utils/src/utils_list.o" \
"hal/src/hal_usart_os.o" \
"hal/utils/src/utils_assert.o" \
"hpl/mcan/hpl_mcan.o" \
"hpl/pmc/hpl_pmc.o" \
"hal/src/hal_spi_m_os.o" \
"stdio_start.o" \
"hal/src/hal_init.o" \
"hpl/twihs/hpl_twihs.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.o" \
"rtos_start.o" \
"sd_mmc/sd_mmc.o" \
"hpl/rtc/hpl_rtc.o" \
"examples/driver_examples.o" \
"led_flasher_main.o" \
"driver_init.o" \
"same70b/gcc/gcc/startup_same70q21b.o" \
"same70b/gcc/system_same70q21b.o" \
"hal/utils/src/utils_ringbuffer.o" \
"hal/src/hal_gpio.o" \
"hal/src/hal_qspi_sync.o" \
"hpl/hsmci/hpl_hsmci.o" \
"hal/src/hal_mci_sync.o" \
"hal/src/hal_sleep.o" \
"atmel_start.o" \
"hpl/core/hpl_core_m7_base.o" \
"hal/src/hal_atomic.o" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/tasks.o" \
"stdio_redirect/gcc/write.o"

# List the directories containing header files
DIR_INCLUDES +=  \
-I"../" \
-I"../config" \
-I"../examples" \
-I"../hal/include" \
-I"../hal/utils/include" \
-I"../hpl/core" \
-I"../hpl/hsmci" \
-I"../hpl/mcan" \
-I"../hpl/pio" \
-I"../hpl/pmc" \
-I"../hpl/qspi" \
-I"../hpl/rtc" \
-I"../hpl/spi" \
-I"../hpl/twihs" \
-I"../hpl/usart" \
-I"../hpl/xdmac" \
-I"../hri" \
-I"../" \
-I"../config" \
-I"../thirdparty/RTOS" \
-I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0" \
-I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/include" \
-I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM7" \
-I"../thirdparty/RTOS/freertos/FreeRTOSV10.0.0/module_config" \
-I"../" \
-I"../config" \
-I"../sd_mmc" \
-I"../" \
-I"../config" \
-I"../stdio_redirect" \
-I"../" \
-I"../CMSIS/Core/Include" \
-I"../same70b/include"

# List the dependency files
DEPS := $(OBJS:%.o=%.d)

DEPS_AS_ARGS +=  \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/event_groups.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/croutine.d" \
"hal/src/hal_calendar.d" \
"stdio_redirect/stdio_io.d" \
"hal/src/hal_mci_sync.d" \
"hal/src/hal_io.d" \
"hal/src/hal_can_async.d" \
"hpl/qspi/hpl_qspi.d" \
"hpl/usart/hpl_usart.d" \
"hal/utils/src/utils_syscalls.d" \
"sd_mmc_start.d" \
"hpl/xdmac/hpl_xdmac.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/timers.d" \
"hal/utils/src/utils_event.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM7/port.d" \
"sd_mmc/sd_mmc.d" \
"hal/src/hal_i2c_m_os.d" \
"hal/src/hal_usart_os.d" \
"hal/utils/src/utils_list.d" \
"stdio_redirect/gcc/write.d" \
"hal/utils/src/utils_assert.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/stream_buffer.d" \
"hal/src/hal_delay.d" \
"hpl/core/hpl_init.d" \
"hpl/spi/hpl_spi.d" \
"stdio_start.d" \
"hal/src/hal_spi_m_os.d" \
"same70b/gcc/gcc/startup_same70q21b.d" \
"hpl/twihs/hpl_twihs.d" \
"hal/src/hal_init.d" \
"hpl/mcan/hpl_mcan.d" \
"hpl/pmc/hpl_pmc.d" \
"driver_init.d" \
"led_flasher_main.d" \
"stdio_redirect/gcc/read.d" \
"rtos_start.d" \
"examples/driver_examples.d" \
"hpl/rtc/hpl_rtc.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/list.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/queue.d" \
"hpl/hsmci/hpl_hsmci.d" \
"hal/src/hal_qspi_sync.d" \
"hal/src/hal_sleep.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.d" \
"hal/utils/src/utils_ringbuffer.d" \
"same70b/gcc/system_same70q21b.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/MemMang/heap_4.d" \
"hal/src/hal_gpio.d" \
"atmel_start.d" \
"hal/src/hal_atomic.d" \
"hpl/core/hpl_core_m7_base.d" \
"thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/tasks.d" \
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
	@echo Invoking: ARM/GNU Linker
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -o $(OUTPUT_FILE_NAME).elf $(OBJS_AS_ARGS) -Wl,--start-group -lm -Wl,--end-group -mthumb \
-Wl,-Map="$(OUTPUT_FILE_NAME).map" --specs=nano.specs -Wl,--gc-sections -mcpu=cortex-m7 \
 \
-T"../same70b/gcc/gcc/same70q21b_flash.ld" \
-L"../same70b/gcc/gcc"
	@echo Finished building target: $@

	"arm-none-eabi-objcopy" -O binary "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).bin"
	"arm-none-eabi-objcopy" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  \
        "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).hex"
	"arm-none-eabi-objcopy" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma \
        .eeprom=0 --no-change-warnings -O binary "$(OUTPUT_FILE_NAME).elf" \
        "$(OUTPUT_FILE_NAME).eep" || exit 0
	"arm-none-eabi-objdump" -h -S "$(OUTPUT_FILE_NAME).elf" > "$(OUTPUT_FILE_NAME).lss"
	"arm-none-eabi-size" "$(OUTPUT_FILE_NAME).elf"

	

# Compiler targets




%.o: %.c
	@echo Building file: $<
	@echo ARM/GNU C Compiler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAME70Q21B__ -mcpu=cortex-m7 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.s
	@echo Building file: $<
	@echo ARM/GNU Assembler
	$(QUOTE)arm-none-eabi-as$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAME70Q21B__ -mcpu=cortex-m7 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.S
	@echo Building file: $<
	@echo ARM/GNU Preprocessing Assembler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAME70Q21B__ -mcpu=cortex-m7 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
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
	rm -f $(OUTPUT_FILE_NAME).a $(OUTPUT_FILE_NAME).hex $(OUTPUT_FILE_NAME).bin \
        $(OUTPUT_FILE_NAME).lss $(OUTPUT_FILE_NAME).eep $(OUTPUT_FILE_NAME).map \
        $(OUTPUT_FILE_NAME).srec