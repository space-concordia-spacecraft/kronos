#include "ks_uart.h"

namespace kronos {
    ComponentUartDriver::ComponentUartDriver(const std::string& name,
                                             uint32_t peripheralId,
                                             uint32_t rxPin,
                                             uint32_t rxPinFunction,
                                             uint32_t txPinFunction,
                                             uint32_t txPin,
                                             void *const baseAddress) :
    ComponentPassive(name),
    m_PeripheralId(peripheralId),
    m_RxPin(rxPin),
    m_RxPinFunction(rxPinFunction),
    m_TxPin(txPin),
    m_TxPinFunction(txPinFunction),
    m_BaseAddress(baseAddress) {}

    KsResult ComponentUartDriver::Init() {
        _pmc_enable_periph_clock(m_PeripheralId);

        usart_sync_init(&UART_DESCRIPTOR, m_BaseAddress, _uart_get_usart_sync());

        gpio_set_pin_function(m_RxPin, m_RxPinFunction);
        gpio_set_pin_function(m_TxPin, m_TxPinFunction);
    }

    KsCmdResult ComponentUartDriver::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_READ:

                break;
            case KS_EVENT_CODE_WRITE:
                uint8_t* data = reinterpret_cast<uint8_t*>(message.data);
                Write(data, sizeof(data));
                delete data;
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    KsResult ComponentUartDriver::Write(const uint8_t* data, size_t len) {
        usart_serial_write_packet(m_UsartInstance, data, len);
        return KS_SUCCESS;
    }
}