#pragma once

#include <string>

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_dac_sync.h>

#include <hal_mci_sync.h>

#include <hal_spi_m_sync.h>
#include <hpl_spi_base.h>

#include <hal_usart_sync.h>
#include <hpl_uart_base.h>

#include <hal_can_async.h>

#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>

#include <samv71q21.h>

#include "pinout.h"

namespace kronos {
    class Driver {
    public:
        Driver(const std::string& name);

        virtual void Init() = 0;
        virtual void Read() = 0;
        virtual void Write() = 0;
    protected:
        std::string m_Name;
    };

    class DriverGPIO : public Driver {
    public:
        DriverGPIO(const std::string&   name,
                   uint8_t              pin,
                   gpio_direction       gpioDirection,
                   bool                 gpioLevel,
                   gpio_pull_mode       gpioPullMode,
                   uint32_t             gpioFunction);

        void Init() override;
        void Read() override;
        void Write() override;

    private:
        uint8_t m_Pin;
        gpio_direction m_Direction;
        bool m_Level;
        gpio_pull_mode m_PullMode;
        uint32_t m_Function;
    };

    class DriverUART : public Driver {
    public:
        DriverUART(const std::string& name,
                   uint32_t peripheralId,
                   uint32_t rxPin,
                   uint32_t rxPinFunction,
                   uint32_t txPin,
                   uint32_t txPinFunction,
                   void *const baseAddress);

        void Init() override;
        void Read() override;
        void Write() override;
    private:
        struct usart_sync_descriptor m_Descriptor;

        uint32_t m_PeripheralId;
        uint32_t m_RxPin;
        uint32_t m_RxPinFunction;
        uint32_t m_TxPin;
        uint32_t m_TxPinFunction;
        void *const m_BaseAddress;
    };

    class DriverCAN : public Driver {
    private:
        DriverCAN(const std::string& name,
                  uint32_t m_PeripheralId,
                  uint32_t rxPin,
                  uint32_t rxPinFunction,
                  uint32_t txPin,
                  uint32_t txPinFunction,
                  void *const baseAddress);

        void Init() override;
        void Read() override;
        void Write() override;

    private:
        struct can_async_descriptor m_Descriptor;

        int32_t m_PeripheralId;
        uint32_t m_RxPin;
        uint32_t m_RxPinFunction;
        uint32_t m_TxPin;
        uint32_t m_TxPinFunction;
        void *const m_BaseAddress;
    };
}