#pragma once

#include "ks_component_passive.h"

#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hpl_spi_base.h>

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_usart_sync.h>
#include <hpl_uart_base.h>

namespace kronos {
    class ComponentUartDriver : public ComponentPassive {

    public:
        ComponentUartDriver(const std::string& name,
                            uint32_t peripheralId,
                            uint32_t rxPin,
                            uint32_t rxPinFunction,
                            uint32_t txPin,
                            uint32_t txPinFunction,
                            void *const baseAddress);
        KsResult Init() override;

        KsCmdResult ProcessEvent(const EventMessage& message) override;

    private:
        KsResult Read();
        KsResult Write(const uint8_t* data, size_t len);

        struct usart_sync_descriptor UART_DESCRIPTOR;

        uint32_t m_PeripheralId;
        uint32_t m_RxPin;
        uint32_t m_RxPinFunction;
        uint32_t m_TxPin;
        uint32_t m_TxPinFunction;
        void *const m_BaseAddress;
    };
}